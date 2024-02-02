#include "VMTDNodeServer.h"
#include "VMTDNodeServerForm.h"

#include "../../VMTDRepo.h"

#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNodeServer::VMTDNodeServer(QObject *parent, VMTDSettings *settings)
        : QObject{parent}
        , m_settings{settings}
    {
        m_wsServer = new QWebSocketServer("VMTDNodeServer", QWebSocketServer::NonSecureMode, this);
        connect(m_wsServer, &QWebSocketServer::newConnection, this, &VMTDNodeServer::newConnectionSlot);

        connect(m_settings, &VMTDSettings::networkChangedSignal,
                this, &VMTDNodeServer::restartListenSlot);

        m_settings->debugOut(VN_S(VMTDNodeServer) + " created");
    }

    VMTDNodeServer::~VMTDNodeServer()
    {
        if (m_form != nullptr)
            m_form->deleteLater();

        stopListenSlot();

        delete m_wsServer;

        m_settings->debugOut(VN_S(VMTDNodeServer) + " deleted");
    }

    VMTDSettings *VMTDNodeServer::settings() const
    {
        return m_settings;
    }

    QWebSocketServer *VMTDNodeServer::wsServer() const
    {
        return m_wsServer;
    }

    QString VMTDNodeServer::stateString() const
    {
        QString state;

        if (m_wsServer->isListening())
            state = QString("Opened. Clients: %1").arg(WsClientSockets.count());
        else
            state = "Closed";

        return state;
    }

    void VMTDNodeServer::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeServerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNodeServer::startListenSlot()
    {
        QString debugString;

        if (m_wsServer->isListening())
        {
            debugString = VN_S(VMTDNodeServer) + "Server already started";
            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);

            return;
        }

        const auto result = m_wsServer->listen(QHostAddress::Any, m_settings->serverPort());

        if (result)
        {
            debugString = "Server started. Listen port: "
                          + QString::number(m_settings->localPort());
        }
        else
        {
            debugString = "Some problem when starting server. See errors.";
        }

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
    }
    void VMTDNodeServer::stopListenSlot()
    {
        QString debugString;

        if (m_wsServer->isListening())
        {
            m_wsServer->close();

            for (auto socket : qAsConst(WsClientSockets))
                socket->close();

            WsClientSockets.clear();

            debugString = "Server stopped";
        }
        else
        {
            debugString = "Server already stopped";
        }

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);
    }
    void VMTDNodeServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }

    void VMTDNodeServer::sendMessageSlot(QWebSocket *socket, const QByteArray &data)
    {
        if (socket == nullptr)
            return;

        if (socket->state() != QAbstractSocket::ConnectedState)
            return;

        socket->sendBinaryMessage(data);

        auto debugString = QString("Sended to {%1:%2}: %3\n")
                           .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                           .arg(socket->peerPort())
                           .arg(QString(data.toHex()));

        emit showDebugSignal(socket, QTime::currentTime(), debugString);
    }

    void VMTDNodeServer::newConnectionSlot()
    {
        QString debugString;

        QWebSocket *socket = m_wsServer->nextPendingConnection();

        if (socket == nullptr)
        {
            debugString = "Incorrect client socket in {newConnection} slot-function";
            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);

            return;
        }

        WsClientSockets.append(socket);

        connect(socket, &QWebSocket::binaryMessageReceived,
                this, &VMTDNodeServer::binaryMessageReceivedSlot);

        connect(socket, &QWebSocket::disconnected, socket, &QWebSocket::deleteLater);
        connect(socket, &QWebSocket::disconnected, this, &VMTDNodeServer::disconnectedSlot);
        connect(socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                this, &VMTDNodeServer::errorSlot);

        debugString = QString("Handled {newConnection} from: %1:%2")
                      .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                      .arg(socket->peerPort());
        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);

        emit clientConnectedSignal(socket);
    }

    void VMTDNodeServer::binaryMessageReceivedSlot(const QByteArray &data)
    {
        auto socket = dynamic_cast<QWebSocket *>(sender());

        const auto jsonDoc = QJsonDocument::fromJson(data);

        const auto debugString = QString("Received from {%1:%2}:\n")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort())
                                 .arg(QString(data.toHex()));

        emit showDebugSignal(socket, QTime::currentTime(), debugString);

        emit receiveMessageSignal(socket, data);
    }

    void VMTDNodeServer::errorSlot(QAbstractSocket::SocketError error)
    {
        auto socket = dynamic_cast<QWebSocket *>(sender());

        if (socket == nullptr)
            return;

        const auto errorString = QString("Error {%1:%2}: %3")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort())
                                 .arg(QVariant::fromValue(error).toString());

        emit showDebugSignal(socket, QTime::currentTime(), errorString);
    }

    void VMTDNodeServer::connectedSlot()
    {
        // do nothing
    }
    void VMTDNodeServer::disconnectedSlot()
    {
        QString debugString;

        QWebSocket *socket = dynamic_cast<QWebSocket *>(sender());

        if (socket == nullptr)
        {
            debugString = "Incorrect client socket in {disconnected} slot-function";

            emit showDebugSignal(nullptr, QTime::currentTime(), debugString);

            return;
        }

        debugString = QString("Handled {disconnected} from: %1:%2")
                      .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                      .arg(socket->peerPort());

        emit showDebugSignal(nullptr, QTime::currentTime(), debugString);

        emit clientDisconnectedSignal(socket);

        WsClientSockets.removeOne(socket);
    }
}

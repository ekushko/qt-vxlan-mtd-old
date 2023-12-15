#include "VMTDNodeServer.h"
#include "VMTDNodeServerForm.h"

#include "../VMTDRepo.h"

#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNodeServer::VMTDNodeServer(QObject *parent, VMTDSettings *settings)
        : QObject{parent}
        , m_settings{settings}
    {
        m_wsServer = new QWebSocketServer("WebSocketServer", QWebSocketServer::NonSecureMode, this);
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

    void VMTDNodeServer::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeServerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDNodeServer::settings() const
    {
        return m_settings;
    }

    QWebSocketServer *VMTDNodeServer::wsServer() const
    {
        return m_wsServer;
    }

    QString VMTDNodeServer::serverErrors() const
    {
        return m_serverErrors;
    }
    void VMTDNodeServer::clearServerErrors()
    {
        m_serverErrors.clear();
    }

    QString VMTDNodeServer::stateString() const
    {
        QString state;

        if (m_wsServer->isListening())
            state = QString("Открыт. Клиентов: %1").arg(WsClientSockets.count());
        else
            state = "Закрыт";

        return state;
    }

    void VMTDNodeServer::startListenSlot()
    {
        QString debugString;

        if (m_wsServer->isListening())
        {
            debugString = VN_S(VMTDNodeServer) + "Server already started";
            emit showDebugSignal(nullptr, debugString);

            return;
        }

        const auto result = m_wsServer->listen(QHostAddress::Any, m_settings->localPort());

        if (result)
        {
            debugString = "Server started. Listen port: "
                          + QString::number(m_settings->localPort());
        }
        else
        {
            debugString = "Some problem when starting server. See errors.";
        }

        emit showDebugSignal(nullptr, debugString);
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

        emit showDebugSignal(nullptr, debugString);
    }
    void VMTDNodeServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }

    void VMTDNodeServer::sendRequestSlot(QWebSocket *socket, const QJsonObject &requestObj)
    {
        if (socket == nullptr)
            return;

        if (socket->state() != QAbstractSocket::ConnectedState)
            return;

        QJsonObject jsonObj;
        jsonObj["jsonrpc"] = "2.0";
        jsonObj["method"] = "cli_ascii";

        jsonObj["request"] = requestObj;
        jsonObj["id"] = m_commandCounter++;

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObj);

        socket->sendBinaryMessage(jsonDoc.toJson());

        emit showDebugSignal(socket, jsonDoc.toJson(QJsonDocument::JsonFormat::Indented));
    }

    void VMTDNodeServer::newConnectionSlot()
    {
        QString debugString;

        QWebSocket *socket = m_wsServer->nextPendingConnection();

        if (socket == nullptr)
        {
            debugString = "Incorrect client socket in {newConnection} slot-function";
            emit showDebugSignal(nullptr, debugString);

            return;
        }

        WsClientSockets.append(socket);

        connect(socket, &QWebSocket::binaryMessageReceived,
                this, &VMTDNodeServer::binaryMessageReceivedSlot);

        connect(socket, &QWebSocket::disconnected, socket, &QWebSocket::deleteLater);
        connect(socket, &QWebSocket::disconnected, this, &VMTDNodeServer::disconnectedSlot);

        connect(socket,
                static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                this, &VMTDNodeServer::errorSlot);

        debugString = QString("Handled {newConnection} from: %1:%2")
                .arg(socket->peerAddress().toString(),
                     socket->peerPort());
        emit showDebugSignal(nullptr, debugString);

        emit clientConnectedSignal(socket);
    }

    void VMTDNodeServer::binaryMessageReceivedSlot(const QByteArray &data)
    {
        QWebSocket *socket = dynamic_cast<QWebSocket *>(sender());

        QJsonDocument jsonDoc;
        jsonDoc.fromJson(data);

        const auto debugString = QString("{%1:%2}:\n")
                .arg(socket->peerAddress().toString(),
                     socket->peerPort()) +
                jsonDoc.toJson(QJsonDocument::JsonFormat::Indented);

        emit showDebugSignal(socket, debugString);
    }

    void VMTDNodeServer::errorSlot(QAbstractSocket::SocketError error)
    {
        QWebSocket *socket = dynamic_cast<QWebSocket *>(sender());

        if (socket == nullptr)
            return;

        const auto debugString = QString("{%1} [%2]: %3")
                .arg(socket->peerAddress().toString(),
                     QTime::currentTime().toString("hh:mm:ss:zzz"),
                     QVariant::fromValue(error).toString());

        if (m_serverErrors.size() > 300)
        {
            int cropSize = m_serverErrors.size()
                           - (m_serverErrors.indexOf('\n') + 1);
            m_serverErrors = m_serverErrors.right(cropSize);
        }

        m_serverErrors.append(debugString);
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

            emit showDebugSignal(nullptr, debugString);

            return;
        }

        debugString = QString("Handled {disconnected} from: %1:%2")
                .arg(socket->peerAddress().toString(),
                     socket->peerPort());

        emit showDebugSignal(nullptr, debugString);

        emit clientDisconnectedSignal(socket);

        WsClientSockets.removeOne(socket);
    }
}

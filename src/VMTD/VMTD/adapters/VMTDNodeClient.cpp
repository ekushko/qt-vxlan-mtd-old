#include "VMTDNodeClient.h"
#include "VMTDNodeClientForm.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace VMTDLib
{
    VMTDNodeClient::VMTDNodeClient(QObject *parent, VMTDSettings *settings)
        : QObject{parent}
        , m_settings{settings}
    {
        connect(m_settings, &VMTDSettings::networkChangedSignal,
                this, &VMTDNodeClient::reconnectSocketSlot);

        m_socket = new QWebSocket(m_settings->debugName(), QWebSocketProtocol::VersionLatest, this);
        connect(m_socket, &QWebSocket::binaryMessageReceived,
                this, &VMTDNodeClient::binaryMessageReceivedSlot);
        connect(m_socket, &QWebSocket::connected, this, &VMTDNodeClient::connectedSlot);
        connect(m_socket, &QWebSocket::disconnected, this, &VMTDNodeClient::disconnectedSlot);
        connect(m_socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>
                (&QWebSocket::error), this, &VMTDNodeClient::errorSlot);
        connect(m_socket, &QWebSocket::stateChanged, this, &VMTDNodeClient::stateChangedSlot);

        m_reconnectTimer.setParent(this);
        connect(&m_reconnectTimer, &QTimer::timeout, this, &VMTDNodeClient::reconnectTimerTickSlot);
        m_reconnectTimer.start(m_settings->reconnectInterval());
    }

    VMTDNodeClient::~VMTDNodeClient()
    {
        if (m_form != nullptr)
            m_form->deleteLater();

        disconnectSocketSlot();

        m_socket->deleteLater();
    }

    void VMTDNodeClient::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeClientForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDNodeClient::settings() const
    {
        return m_settings;
    }

    QWebSocket *VMTDNodeClient::socket() const
    {
        return m_socket;
    }

    QString VMTDNodeClient::socketErrors() const
    {
        return m_socketErrors;
    }
    void VMTDNodeClient::clearSocketErrors()
    {
        m_socketErrors.clear();
    }

    QString VMTDNodeClient::stateString() const
    {
        QString state;

        if (m_socket->state() != QAbstractSocket::ConnectedState)
            state = "Есть соединение";
        else
            state = "Нет соединения";

        return state;
    }

    void VMTDNodeClient::connectSocketSlot()
    {
        if (m_socket->state() != QAbstractSocket::UnconnectedState)
            return;

        QUrl url;
        url.setHost(m_settings->serverIp());
        url.setPort(m_settings->serverPort());
        url.setScheme("ws");

        m_socket->open(url);
    }
    void VMTDNodeClient::disconnectSocketSlot()
    {
        if (m_socket->state() == QAbstractSocket::UnconnectedState)
            return;

        m_socket->close();
    }
    void VMTDNodeClient::reconnectSocketSlot()
    {
        disconnectedSlot();
        connectSocketSlot();
    }

    void VMTDNodeClient::sendMessageSlot(QWebSocket *socket, const QJsonObject &messageObj)
    {
        if (m_socket != socket
            || m_socket->state() != QAbstractSocket::ConnectedState)
            return;


        QJsonObject jsonObj;
        jsonObj["jsonrpc"] = "2.0";
        jsonObj["method"] = "cli_ascii";

        jsonObj["message"] = messageObj;
        jsonObj["id"] = m_commandCounter++;

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObj);

        m_socket->sendBinaryMessage(jsonDoc.toJson());

        emit showDebugSignal(m_socket, jsonDoc.toJson(QJsonDocument::JsonFormat::Indented));
    }


    void VMTDNodeClient::binaryMessageReceivedSlot(const QByteArray &data)
    {
        QJsonDocument jsonDoc;
        jsonDoc.fromJson(data);

        const auto debugString = QString("{%1:%2}:\n")
                .arg(m_socket->peerAddress().toString(),
                     m_socket->peerPort()) +
                jsonDoc.toJson(QJsonDocument::JsonFormat::Indented);

        emit showDebugSignal(m_socket, debugString);

        emit receiveMessageSignal(jsonDoc.object());
    }

    void VMTDNodeClient::connectedSlot()
    {
        emit showDebugSignal(m_socket, "Socket connected!");
    }
    void VMTDNodeClient::disconnectedSlot()
    {
        emit showDebugSignal(m_socket, "Socket disconnected!");
    }

    void VMTDNodeClient::errorSlot(QAbstractSocket::SocketError error)
    {
        const auto errStr = QString("[%1]: %2")
                .arg(QTime::currentTime().toString("hh:mm:ss:zzz"),
                     QVariant::fromValue(error).toString());

        if (m_socketErrors.size() > 300)
        {
            int cropSize = m_socketErrors.size() - (m_socketErrors.indexOf('\n') + 1);
            m_socketErrors = m_socketErrors.right(cropSize);
        }

        m_socketErrors.append(errStr);
    }

    void VMTDNodeClient::stateChangedSlot(QAbstractSocket::SocketState state)
    {
        const auto debugString = QString("{%1:%2}:\n")
                .arg(m_socket->peerAddress().toString(),
                     m_socket->peerPort()) +
                QVariant::fromValue(state).toString();

        emit showDebugSignal(m_socket, debugString);
    }

    void VMTDNodeClient::reconnectTimerTickSlot()
    {
        if (!m_settings->shouldReconnect())
            return;

        if (m_socket->state() != QAbstractSocket::ConnectedState
            && m_socket->state() != QAbstractSocket::ConnectingState
            && m_socket->state() != QAbstractSocket::HostLookupState)
        {
            emit showDebugSignal(m_socket, "Socket is not connected. Trying to reconnect");
        }
    }
}

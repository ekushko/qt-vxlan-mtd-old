#include "VMTDNodeClient.h"
#include "VMTDNodeClientForm.h"

#include "../../VMTDRepo.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace VMTDLib
{
    VMTDNodeClient::VMTDNodeClient(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDNodeClient) + " | Constructor called");

        m_socket = new QWebSocket(m_settings->debugName(), QWebSocketProtocol::VersionLatest, this);
        connect(m_socket, &QWebSocket::textMessageReceived,
                this, &VMTDNodeClient::textMessageReceivedSlot);
        connect(m_socket, &QWebSocket::connected, this, &VMTDNodeClient::connectedSlot);
        connect(m_socket, &QWebSocket::disconnected, this, &VMTDNodeClient::disconnectedSlot);
        connect(m_socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>
                (&QWebSocket::error), this, &VMTDNodeClient::errorSlot);
        connect(m_socket, &QWebSocket::stateChanged, this, &VMTDNodeClient::stateChangedSlot);

        m_reconnectTimer.setParent(this);
        connect(&m_reconnectTimer, &QTimer::timeout, this, &VMTDNodeClient::reconnectTimerTickSlot);

        if (m_settings->shouldReconnect())
            m_reconnectTimer.start(m_settings->reconnectInterval());

        m_settings->creationOut(VN_S(VMTDNodeClient) + " | Constructor finished");
    }

    VMTDNodeClient::~VMTDNodeClient()
    {
        m_settings->creationOut(VN_S(VMTDNodeClient) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        disconnectSocketSlot();

        m_socket->deleteLater();

        m_settings->creationOut(VN_S(VMTDNodeClient) + " | Destructor finished");
    }

    VMTDSettings *VMTDNodeClient::settings() const
    {
        return m_settings;
    }

    QWebSocket *VMTDNodeClient::socket() const
    {
        return m_socket;
    }

    QString VMTDNodeClient::stateString() const
    {
        QString state;

        if (m_socket->state() != QAbstractSocket::ConnectedState)
            state = "Connected";
        else
            state = "Disconnected";

        return state;
    }

    void VMTDNodeClient::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDNodeClientForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
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

        emit socketConnectedSignal(m_socket);
    }
    void VMTDNodeClient::disconnectSocketSlot()
    {
        if (m_socket->state() == QAbstractSocket::UnconnectedState)
            return;

        emit socketDisconnectedSignal(m_socket);

        m_socket->close();
    }
    void VMTDNodeClient::reconnectSocketSlot()
    {
        disconnectedSlot();
        connectSocketSlot();
    }

    void VMTDNodeClient::sendDataSlot(QWebSocket *socket, const QString &data)
    {
        if (m_socket != socket
            || m_socket->state() != QAbstractSocket::ConnectedState)
            return;

        m_socket->sendTextMessage(data);

        auto debugString = QString("Sended to {%1:%2}: %3")
                           .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                           .arg(socket->peerPort())
                           .arg(data);

        emit showDebugSignal(socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);
    }


    void VMTDNodeClient::textMessageReceivedSlot(const QString &data)
    {
        const auto debugString = QString("Received from {%1:%2}: %3\n")
                                 .arg(QHostAddress(m_socket->peerAddress().toIPv4Address()).toString())
                                 .arg(m_socket->peerPort())
                                 .arg(data);

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);

        emit receiveMessageSignal(m_socket, data);
    }

    void VMTDNodeClient::connectedSlot()
    {
        const auto debugString = QString("Socket connected!");

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);
    }
    void VMTDNodeClient::disconnectedSlot()
    {
        const auto debugString = QString("Socket disconnected!");

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);
    }

    void VMTDNodeClient::errorSlot(QAbstractSocket::SocketError error)
    {
        const auto errorString = QString("Error {%1:%2}: %3")
                                 .arg(m_settings->serverIp())
                                 .arg(m_settings->serverPort())
                                 .arg(QVariant::fromValue(error).toString());

        emit showDebugSignal(m_socket, QTime::currentTime(), errorString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + errorString);
    }

    void VMTDNodeClient::stateChangedSlot(QAbstractSocket::SocketState state)
    {
        const auto debugString = QString("{%1:%2}: ")
                                 .arg(m_settings->serverIp())
                                 .arg(m_settings->serverPort())
                                 + QVariant::fromValue(state).toString();

        emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);
    }

    void VMTDNodeClient::reconnectTimerTickSlot()
    {
        if (!m_settings->shouldReconnect())
            return;

        if (m_socket->state() != QAbstractSocket::ConnectedState
            && m_socket->state() != QAbstractSocket::ConnectingState
            && m_socket->state() != QAbstractSocket::HostLookupState)
        {
            const auto debugString = QString("Socket is not connected. Trying to reconnect");

            connectSocketSlot();

            emit showDebugSignal(m_socket, QTime::currentTime(), debugString);
            m_settings->debugOut(VN_S(VMTDNodeClient) + " | " + debugString);
        }
    }
}

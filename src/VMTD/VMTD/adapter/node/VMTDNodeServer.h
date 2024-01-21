#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>

#include "../../VMTDSettings.h"

namespace VMTDLib
{
    class VMTDNodeServerForm;

    class VMTD_SHARED VMTDNodeServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDNodeServer(QObject *parent, VMTDSettings *settings);
        ~VMTDNodeServer();

        VMTDSettings *settings() const;

        QWebSocketServer *wsServer() const;

        QList<QWebSocket *> WsClientSockets;

        QString stateString() const;

    signals:

        void clientConnectedSignal(QWebSocket *socket);
        void clientDisconnectedSignal(QWebSocket *socket);

        void receiveMessageSignal(const QJsonObject &messageObj);

        void showDebugSignal(QWebSocket *socket, const QTime &time, const QString &text);

    public slots:

        void showFormSlot();

        void   startListenSlot();
        void    stopListenSlot();
        void restartListenSlot();

        void sendMessageSlot(QWebSocket *socket, const QJsonObject &messageObj);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDNodeServerForm> m_form;

        QWebSocketServer *m_wsServer;

        int m_commandCounter = 1;

    private slots:

        void newConnectionSlot();

        void binaryMessageReceivedSlot(const QByteArray &data);

        void errorSlot(QAbstractSocket::SocketError error);

        void    connectedSlot();
        void disconnectedSlot();
    };
}

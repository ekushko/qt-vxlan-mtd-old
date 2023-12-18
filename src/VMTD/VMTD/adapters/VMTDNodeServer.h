#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>

#include "../VMTDSettings.h"

namespace VMTDLib
{
    class VMTDNodeServerForm;

    class VMTDNodeServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDNodeServer(QObject *parent, VMTDSettings *settings);
        ~VMTDNodeServer();

        void showForm();

        VMTDSettings *settings() const;

        QWebSocketServer *wsServer() const;

        QList<QWebSocket *> WsClientSockets;

        QString   serverErrors() const;
        void clearServerErrors();

        QString stateString() const;

    signals:

        void clientConnectedSignal(QWebSocket *socket);
        void clientDisconnectedSignal(QWebSocket *socket);

        void showDebugSignal(QWebSocket *socket, const QString &text);

    public slots:

        void   startListenSlot();
        void    stopListenSlot();
        void restartListenSlot();

        void sendMessageSlot(QWebSocket *socket, const QJsonObject &messageObj);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDNodeServerForm> m_form;

        QWebSocketServer *m_wsServer;

        QString m_serverErrors;

        int m_commandCounter = 0;

    private slots:

        void newConnectionSlot();

        void binaryMessageReceivedSlot(const QByteArray &data);

        void errorSlot(QAbstractSocket::SocketError error);

        void    connectedSlot();
        void disconnectedSlot();
    };
}

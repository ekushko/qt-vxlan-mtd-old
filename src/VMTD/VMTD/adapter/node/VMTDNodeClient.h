#pragma once

#include "../../VMTDSettings.h"

#include <QWebSocket>
#include <QPointer>
#include <QTimer>

namespace VMTDLib
{
    class VMTDNodeClientForm;

    class VMTD_SHARED VMTDNodeClient : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(VMTDNodeClient)

    public:

        VMTDNodeClient(QObject *parent, VMTDSettings *settings);
        ~VMTDNodeClient();

        VMTDSettings *settings() const;

        QWebSocket *socket() const;

        QString stateString() const;

    signals:

        void receiveMessageSignal(QWebSocket *socket, const QString &data);

        void showDebugSignal(QWebSocket *socket, const QTime &time, const QString &text);

        void socketConnectedSignal(QWebSocket *socket);
        void socketDisconnectedSignal(QWebSocket *socket);

    public slots:

        void showFormSlot();

        void    connectSocketSlot();
        void disconnectSocketSlot();
        void  reconnectSocketSlot();

        void sendDataSlot(QWebSocket *socket, const QString &data);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDNodeClientForm> m_form;

        QWebSocket *m_socket;

        QTimer m_reconnectTimer;

        int m_commandCounter = 1;

    private slots:

        void textMessageReceivedSlot(const QString &data);

        void    connectedSlot();
        void disconnectedSlot();

        void errorSlot(QAbstractSocket::SocketError error);
        void stateChangedSlot(QAbstractSocket::SocketState stateString);

        void reconnectTimerTickSlot();
    };
}

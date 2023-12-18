#pragma once

#include "../VMTDSettings.h"

#include <QWebSocket>
#include <QPointer>
#include <QTimer>

namespace VMTDLib
{
    class VMTDNodeClientForm;

    class VMTDNodeClient : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(VMTDNodeClient)

    public:

        VMTDNodeClient(QObject *parent, VMTDSettings *settings);
        ~VMTDNodeClient();

        void showForm();

        VMTDSettings *settings() const;

        QWebSocket *socket() const;

        QString   socketErrors() const;
        void clearSocketErrors();

        QString stateString() const;

    signals:

        void receiveDataSignal(const QByteArray &data);

        void showDebugSignal(QWebSocket *socket, const QString &text);

    public slots:

        void    connectSocketSlot();
        void disconnectSocketSlot();
        void  reconnectSocketSlot();

        void sendRequestSlot(QWebSocket *socket, const QJsonObject &requestObj);

    private:

        VMTDSettings *m_settings;

        QPointer<VMTDNodeClientForm> m_form;

        QWebSocket *m_socket;

        QString m_socketErrors;

        QTimer m_reconnectTimer;

        int m_commandCounter = 0;

    private slots:

        void binaryMessageReceivedSlot(const QByteArray &data);

        void    connectedSlot();
        void disconnectedSlot();

        void errorSlot(QAbstractSocket::SocketError error);
        void stateChangedSlot(QAbstractSocket::SocketState stateString);

        void reconnectTimerTickSlot();
    };
}

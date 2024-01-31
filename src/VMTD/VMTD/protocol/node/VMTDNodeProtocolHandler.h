#pragma once

#include "../VMTDProtocolHandler.h"

#include <QWebSocket>

namespace VMTDLib
{
    class VMTDNodeProtocolHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDNodeProtocolHandler(QObject *parent, VMTDSettings *settings,
                                VMTDNodeDevice *device,
                                QWebSocket *socket);

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString name() const override;

        int queueLength() const override;

    signals:

        void sendMessageSignal(QWebSocket *socket, const QJsonObject &messageObj);

    public slots:

        void receiveMessageSlot(QWebSocket *socket, const QJsonObject &messageObj);

        void clearQueueSlot() override;

    private:

        VMTDNodeDevice *m_device;

        QWebSocket *m_socket;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}

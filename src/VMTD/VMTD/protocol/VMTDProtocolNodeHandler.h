#pragma once

#include "VMTDProtocolHandler.h"

#include <QWebSocket>

namespace VMTDLib
{
    class VMTDProtocolNodeHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDProtocolNodeHandler(QObject *parent, VMTDSettings *settings,
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
    };
}

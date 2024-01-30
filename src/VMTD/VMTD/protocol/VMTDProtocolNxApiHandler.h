#pragma once

#include "VMTDProtocolHandler.h"

#include "../adapter/nxapi/VMTDNxApiAdapter.h"

namespace VMTDLib
{
    class VMTDProtocolNxApiHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDProtocolNxApiHandler(QObject *parent, VMTDSettings *settings,
                                 VMTDNxApiDevice *device,
                                 VMTDNxApiAdapter *adapter);

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString name() const override;

        int queueLength() const override;

    signals:

        void sendCommandSignal(const QStringList &commands);

    public slots:

        void clearQueueSlot() override;

    private:

        VMTDNxApiDevice *m_device;

        VMTDNxApiAdapter *m_adapter;

    };
}

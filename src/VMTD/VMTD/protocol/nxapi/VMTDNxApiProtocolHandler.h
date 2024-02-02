#pragma once

#include "../VMTDProtocolHandler.h"

#include "../../adapter/nxapi/VMTDNxApiAdapter.h"

namespace VMTDLib
{
    class VMTDNxApiProtocolHandlerForm;

    class VMTDNxApiProtocolHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDNxApiProtocolHandler(QObject *parent, VMTDSettings *settings,
                                 VMTDNxApiDevice *device,
                                 VMTDNxApiAdapter *adapter);
        ~VMTDNxApiProtocolHandler();

        void showForm() override;

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString name() const override;

        int queueLength() const override;

    signals:

        void sendCommandSignal(const QStringList &command);

        void checkConnectionSignal();

    public slots:

        void appendCommandSlot(const QStringList &command);

        void commandExecutedSlot(bool ok);

        void clearQueueSlot() override;

    private:

        QPointer<VMTDNxApiProtocolHandlerForm> m_form;

        VMTDNxApiDevice *m_device;

        VMTDNxApiAdapter *m_adapter;

        QQueue<QStringList> m_queue;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}

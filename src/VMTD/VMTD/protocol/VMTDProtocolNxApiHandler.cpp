#include "VMTDProtocolNxApiHandler.h"

namespace VMTDLib
{
    VMTDProtocolNxApiHandler::VMTDProtocolNxApiHandler(QObject *parent, VMTDSettings *settings,
                                                       VMTDNxApiDevice *device,
                                                       VMTDNxApiAdapter *adapter)
        : VMTDProtocolHandler(parent, settings)
        , m_device(device)
        , m_adapter(adapter)
    {

    }

    void VMTDProtocolNxApiHandler::checkConnection()
    {
        m_adapter->checkConnectionSlot();

        QTimer::singleShot(m_adapter->ticketTimeoutInterval(), [this]()
        {
            m_device->setOnline(m_adapter->isConnected());
        });
    }

    QString VMTDProtocolNxApiHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->url().toString(QUrl::RemoveUserInfo));
    }

    int VMTDProtocolNxApiHandler::queueLength() const
    {
        return 0;
    }

    void VMTDProtocolNxApiHandler::clearQueueSlot()
    {
        // do nothing
    }
}

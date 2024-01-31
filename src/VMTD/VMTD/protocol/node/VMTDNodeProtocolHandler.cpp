#include "VMTDNodeProtocolHandler.h"

namespace VMTDLib
{
    VMTDNodeProtocolHandler::VMTDNodeProtocolHandler(QObject *parent, VMTDSettings *settings,
                                                     VMTDNodeDevice *device,
                                                     QWebSocket *socket)
        : VMTDProtocolHandler(parent, settings)
        , m_device(device)
        , m_socket(socket)
    {
        m_ticketTimeoutTimer.setInterval(m_device->ticketTimeoutInterval());

        m_checkQueueTimer.setInterval(m_device->checkQueueInterval());
        m_checkQueueTimer.start();
    }

    void VMTDNodeProtocolHandler::checkConnection()
    {
        // в разработке
    }

    QString VMTDNodeProtocolHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->ip());
    }

    int VMTDNodeProtocolHandler::queueLength() const
    {
        return 0;
    }

    void VMTDNodeProtocolHandler::receiveMessageSlot(QWebSocket *socket, const QJsonObject &messageObj)
    {
        Q_UNUSED(socket)
        Q_UNUSED(messageObj)

        // в разработке
    }

    void VMTDNodeProtocolHandler::clearQueueSlot()
    {
        // do nothing
    }

    void VMTDNodeProtocolHandler::checkQueueTimerSlot()
    {

    }

    void VMTDNodeProtocolHandler::ticketTimeoutSlot()
    {

    }
}

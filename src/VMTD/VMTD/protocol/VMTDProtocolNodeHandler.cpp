#include "VMTDProtocolNodeHandler.h"

namespace VMTDLib
{
    VMTDProtocolNodeHandler::VMTDProtocolNodeHandler(QObject *parent, VMTDSettings *settings,
                                                     VMTDNodeDevice *device,
                                                     QWebSocket *socket)
        : VMTDProtocolHandler(parent, settings)
        , m_device(device)
        , m_socket(socket)
    {

    }

    void VMTDProtocolNodeHandler::checkConnection()
    {
        // в разработке
    }

    QString VMTDProtocolNodeHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->ip());
    }

    int VMTDProtocolNodeHandler::queueLength() const
    {
        return 0;
    }

    void VMTDProtocolNodeHandler::receiveMessageSlot(QWebSocket *socket, const QJsonObject &messageObj)
    {
        Q_UNUSED(socket)
        Q_UNUSED(messageObj)

        // в разработке
    }

    void VMTDProtocolNodeHandler::clearQueueSlot()
    {
        // do nothing
    }

    void VMTDProtocolNodeHandler::checkQueueTimerSlot()
    {

    }

    void VMTDProtocolNodeHandler::ticketTimeoutSlot()
    {

    }
}

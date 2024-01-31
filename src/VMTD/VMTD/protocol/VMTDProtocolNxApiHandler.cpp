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
        connect(this, &VMTDProtocolNxApiHandler::sendCommandSignal,
                m_adapter, &VMTDNxApiAdapter::sendCommandSlot);
        connect(m_adapter, &VMTDNxApiAdapter::commandExecutedSignal,
                this, &VMTDProtocolNxApiHandler::commandExecutedSlot);

        m_ticketTimeoutTimer.setInterval(m_device->ticketTimeoutInterval());

        m_checkQueueTimer.setInterval(m_device->checkQueueInterval());
        m_checkQueueTimer.start();
    }

    void VMTDProtocolNxApiHandler::checkConnection()
    {
        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        emit checkConnectionSignal();

        emit showDebugSignal(QTime::currentTime(), "Check connection!");

        m_ticketTimeoutTimer.start();
    }

    QString VMTDProtocolNxApiHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->url().toString(QUrl::RemoveUserInfo));
    }

    int VMTDProtocolNxApiHandler::queueLength() const
    {
        return m_queue.length();
    }

    void VMTDProtocolNxApiHandler::appendCommandSlot(const QStringList &command)
    {
        m_queue.enqueue(command);
    }

    void VMTDProtocolNxApiHandler::commandExecutedSlot(bool ok)
    {
        if (m_queueState == EnQueueState::WAIT_FOR_TICKET)
        {
            if (m_ticketTimeoutTimer.isActive())
                m_ticketTimeoutTimer.stop();

            emit showDebugSignal(QTime::currentTime(), QString("Command %1")
                                 .arg((ok ? "executed!" : "failed!")));

            m_device->setOnline(true);

            m_queueState = EnQueueState::READY_TO_SEND;
        }
        else
        {
            emit showDebugSignal(QTime::currentTime(), QString("Unexpected response!"));
        }
    }

    void VMTDProtocolNxApiHandler::clearQueueSlot()
    {
        m_queueState = EnQueueState::READY_TO_SEND;
        m_queue.clear();
    }

    void VMTDProtocolNxApiHandler::checkQueueTimerSlot()
    {
        if (m_queueState != EnQueueState::READY_TO_SEND)
            return;

        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        const auto command = m_queue.dequeue();
        emit showDebugSignal(QTime::currentTime(), QString("Command sent:\n%1")
                             .arg(command.join('\n')));
        emit sendCommandSignal(command);

        m_ticketTimeoutTimer.start();
    }

    void VMTDProtocolNxApiHandler::ticketTimeoutSlot()
    {
        emit showDebugSignal(QTime::currentTime(), "Command not executed!");

        m_queueState = EnQueueState::READY_TO_SEND;

        m_device->setOnline(false);
    }
}

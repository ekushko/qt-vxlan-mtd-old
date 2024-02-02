#include "VMTDNxApiProtocolHandler.h"
#include "VMTDNxApiProtocolHandlerForm.h"

namespace VMTDLib
{
    VMTDNxApiProtocolHandler::VMTDNxApiProtocolHandler(QObject *parent, VMTDSettings *settings,
                                                       VMTDNxApiDevice *device,
                                                       VMTDNxApiAdapter *adapter)
        : VMTDProtocolHandler(parent, settings, EnType::NX_API)
        , m_device(device)
        , m_adapter(adapter)
    {
        m_ticketTimeoutTimer.setInterval(m_device->ticketTimeoutInterval());

        m_checkQueueTimer.setInterval(m_device->checkQueueInterval());
        m_checkQueueTimer.start();
    }

    VMTDNxApiProtocolHandler::~VMTDNxApiProtocolHandler()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    void VMTDNxApiProtocolHandler::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiProtocolHandlerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNxApiProtocolHandler::checkConnection()
    {
        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        emit checkConnectionSignal();

        emit showDebugSignal(QTime::currentTime(), "Check connection!");

        m_ticketTimeoutTimer.start();
    }

    QString VMTDNxApiProtocolHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->url().toString(QUrl::RemoveUserInfo));
    }

    int VMTDNxApiProtocolHandler::queueLength() const
    {
        return m_queue.length();
    }

    void VMTDNxApiProtocolHandler::appendCommandSlot(const QStringList &command)
    {
        m_queue.enqueue(command);
    }

    void VMTDNxApiProtocolHandler::commandExecutedSlot(bool ok)
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

    void VMTDNxApiProtocolHandler::clearQueueSlot()
    {
        m_queueState = EnQueueState::READY_TO_SEND;
        m_queue.clear();
    }

    void VMTDNxApiProtocolHandler::checkQueueTimerSlot()
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

    void VMTDNxApiProtocolHandler::ticketTimeoutSlot()
    {
        emit showDebugSignal(QTime::currentTime(), "Command not executed!");

        m_queueState = EnQueueState::READY_TO_SEND;

        m_device->setOnline(false);
    }
}

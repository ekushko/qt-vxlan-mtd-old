#include "VMTDNxApiProtocolHandler.h"
#include "VMTDNxApiProtocolHandlerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNxApiProtocolHandler::VMTDNxApiProtocolHandler(QObject *parent, VMTDSettings *settings,
                                                       VMTDNxApiDevice *device,
                                                       VMTDNxApiAdapter *adapter)
        : VMTDProtocolHandler(parent, settings, EnType::NX_API)
        , m_device(device)
        , m_adapter(adapter)
    {
        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandler) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandler) + " | Constructor finished");
    }

    VMTDNxApiProtocolHandler::~VMTDNxApiProtocolHandler()
    {
        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandler) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandler) + " | Destructor finished");
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
               .arg(id())
               .arg(m_adapter->url().toString(QUrl::RemoveUserInfo));
    }

    int VMTDNxApiProtocolHandler::queueLength() const
    {
        return m_queue.length();
    }

    void VMTDNxApiProtocolHandler::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDNxApiProtocolHandlerForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNxApiProtocolHandler::appendCommandSlot(const QStringList &command)
    {
        m_queue.enqueue(command);
    }

    void VMTDNxApiProtocolHandler::commandExecutedSlot(bool isOnline, bool hasError)
    {
        if (m_queueState == EnQueueState::WAIT_FOR_TICKET)
        {
            if (m_ticketTimeoutTimer.isActive())
                m_ticketTimeoutTimer.stop();

            if (m_device != nullptr)
                m_device->setOnline(isOnline);

            if (isOnline)
            {
                emit showDebugSignal(QTime::currentTime(), QString("Command %1")
                                     .arg((!hasError ? "executed!" : "failed!")));
            }
            else
            {
                emit showDebugSignal(QTime::currentTime(), "Response not received!");
            }

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
        if (m_queue.isEmpty()
            || m_queueState != EnQueueState::READY_TO_SEND)
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

        if (m_device != nullptr)
            m_device->setOnline(false);
    }
}

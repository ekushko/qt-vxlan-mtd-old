#include "VMTDProtocolHandler.h"

namespace VMTDLib
{
    VMTDProtocolHandler::VMTDProtocolHandler(QObject *parent, VMTDSettings *settings, EnType type)
        : QObject(parent)
        , m_settings(settings)
        , m_type(type)
    {
        m_id = m_settings->generateId();

        m_queueState = EnQueueState::READY_TO_SEND;

        m_checkQueueTimer.setParent(this);
        m_checkQueueTimer.setInterval(m_settings->checkQueueInterval());
        connect(&m_checkQueueTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::checkQueueTimerSlot);
        m_checkQueueTimer.start();

        m_ticketTimeoutTimer.setParent(this);
        m_ticketTimeoutTimer.setInterval(m_settings->ticketTimeoutInterval());
        connect(&m_ticketTimeoutTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::ticketTimeoutSlot);
        m_ticketTimeoutTimer.setSingleShot(true);
    }

    VMTDProtocolHandler::EnQueueState VMTDProtocolHandler::queueState() const
    {
        return m_queueState;
    }

    int VMTDProtocolHandler::id() const
    {
        return m_id;
    }

    VMTDProtocolHandler::EnType VMTDProtocolHandler::type() const
    {
        return m_type;
    }

    void VMTDProtocolHandler::checkConnectionSlot()
    {
        checkConnection();
    }
}

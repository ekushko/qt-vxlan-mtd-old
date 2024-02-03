#include "VMTDProtocolHandler.h"

namespace VMTDLib
{
    VMTDProtocolHandler::VMTDProtocolHandler(QObject *parent, VMTDSettings *settings, EnType type)
        : QObject(parent)
        , m_settings(settings)
        , m_type(type)
    {
        m_queueState = EnQueueState::READY_TO_SEND;

        m_checkQueueTimer.setParent(this);
        connect(&m_checkQueueTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::checkQueueTimerSlot);

        m_ticketTimeoutTimer.setParent(this);
        connect(&m_ticketTimeoutTimer, &QTimer::timeout,
                this, &VMTDProtocolHandler::ticketTimeoutSlot);
        m_ticketTimeoutTimer.setSingleShot(true);
    }

    VMTDProtocolHandler::EnQueueState VMTDProtocolHandler::queueState() const
    {
        return m_queueState;
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

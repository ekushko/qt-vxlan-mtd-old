#pragma once

#include "../VMTDSettings.h"
#include "../net/VMTDNet.h"

#include <QQueue>
#include <QTimer>

namespace VMTDLib
{
    class VMTDProtocolHandler : public QObject
    {
        Q_OBJECT

    public:

        VMTDProtocolHandler(QObject *parent, VMTDSettings *settings);

        // КОНСТАНТЫ

        enum class EnQueueState
        {
            READY_TO_SEND,
            WAIT_FOR_TICKET
        };
        Q_ENUM(EnQueueState)

        // ЛОГИКА

        virtual void checkConnection() = 0;

        // МЕТАДАННЫЕ

        EnQueueState queueState() const;

        // ДАННЫЕ

        virtual QString name() const = 0;

        virtual int queueLength() const = 0;

    signals:

        void showDebugSignal(const QTime &time, const QString &text);

    public slots:

        virtual void clearQueueSlot() = 0;

    protected:

        VMTDSettings *m_settings;

        EnQueueState m_queueState;

        QTimer m_checkQueueTimer;

        QTimer m_ticketTimeoutTimer;

    protected slots:

        virtual void checkQueueTimerSlot() = 0;

        virtual void ticketTimeoutSlot() = 0;
    };
}

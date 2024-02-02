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

        enum class EnType;

        VMTDProtocolHandler(QObject *parent, VMTDSettings *settings, EnType type);

        virtual void showForm() = 0;

        // КОНСТАНТЫ

        enum class EnType
        {
            NX_API,
            NODE
        };
        Q_ENUM(EnType)

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

        EnType type() const;

        virtual QString name() const = 0;

        virtual int queueLength() const = 0;

    signals:

        void showDebugSignal(const QTime &time, const QString &text);

    public slots:

        virtual void checkConnectionSlot();

        virtual void clearQueueSlot() = 0;

    protected:

        VMTDSettings *m_settings;

        EnType m_type;

        EnQueueState m_queueState;

        QTimer m_checkQueueTimer;

        QTimer m_ticketTimeoutTimer;

    protected slots:

        virtual void checkQueueTimerSlot() = 0;

        virtual void ticketTimeoutSlot() = 0;
    };
}

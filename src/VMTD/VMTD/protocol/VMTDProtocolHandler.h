#pragma once

#include "../VMTDSettings.h"
#include "../net/VMTDNet.h"

namespace VMTDLib
{
    class VMTDProtocolHandler : public QObject
    {
        Q_OBJECT

    public:

        VMTDProtocolHandler(QObject *parent, VMTDSettings *settings);

        // ЛОГИКА

        virtual void checkConnection() = 0;

        // ДАННЫЕ

        virtual QString name() const = 0;

        virtual int queueLength() const = 0;

    public slots:

        virtual void clearQueueSlot() = 0;

    protected:

        VMTDSettings *m_settings;

    };
}

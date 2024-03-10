#pragma once

#include "../../VMTDSettings.h"

namespace VMTDLib
{
    class VMTDVRoute
    {
    public:

        explicit VMTDVRoute(VMTDSettings *settings);
        VMTDVRoute(VMTDSettings *settings,
                   const QString &network, uchar mask,
                   const QString &gateway, int metric);
        ~VMTDVRoute();

        // ЛОГИКА

        QString toString() const;

        // ДАННЫЕ

        QString network() const;
        void setNetwork(const QString &network);

        uchar   mask() const;
        void setMask(uchar mask);

        QString gateway() const;
        void setGateway(const QString &gateway);

        int     metric() const;
        void setMetric(int metric);

    private:

        VMTDSettings *m_settings;

        QString m_network;
        uchar m_mask;
        QString m_gateway;
        int m_metric;
    };
}

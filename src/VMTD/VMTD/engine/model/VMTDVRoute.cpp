#include "VMTDVRoute.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDVRoute::VMTDVRoute(VMTDSettings *settings)
        : VMTDVRoute(settings,
                     QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                     QString(NETWORK_TEMPLATE).arg(0).arg(1), 100)
    {

    }

    VMTDVRoute::VMTDVRoute(VMTDSettings *settings,
                           const QString &network, uchar mask,
                           const QString &gateway, int metric)
        : m_settings(settings)
        , m_network(network)
        , m_mask(mask)
        , m_gateway(gateway)
        , m_metric(metric)
    {
        m_settings->creationOut(VN_S(VMTDVRoute) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDVRoute) + " | Constructor finished");
    }

    VMTDVRoute::~VMTDVRoute()
    {
        m_settings->creationOut(VN_S(VMTDVRoute) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDVRoute) + " | Destructor finished");
    }

    QString VMTDVRoute::toString() const
    {
        return QString("%1/%2 via %3 metric %4")
               .arg(m_network)
               .arg(m_mask)
               .arg(m_gateway)
               .arg(m_metric);
    }

    QString VMTDVRoute::network() const
    {
        return m_network;
    }
    void VMTDVRoute::setNetwork(const QString &network)
    {
        m_network = network;
    }

    uchar VMTDVRoute::mask() const
    {
        return m_mask;
    }
    void VMTDVRoute::setMask(uchar mask)
    {
        m_mask = mask;
    }

    QString VMTDVRoute::gateway() const
    {
        return m_gateway;
    }
    void VMTDVRoute::setGateway(const QString &gateway)
    {
        m_gateway = gateway;
    }

    int VMTDVRoute::metric() const
    {
        return m_metric;
    }
    void VMTDVRoute::setMetric(int metric)
    {
        m_metric = metric;
    }
}

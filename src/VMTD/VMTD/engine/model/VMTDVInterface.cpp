#include "VMTDVInterface.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDVInterface::VMTDVInterface(QObject *parent, VMTDSettings *settings,
                                   const QString &interfaceName)
        : QObject(parent)
        , m_settings(settings)
        , m_interfaceName(interfaceName)
    {
        m_settings->creationOut(VN_S(VMTDVInterface) + " | Constructor called");

        m_index = -1;
        m_groupIndex = -1;
        m_ip = QString(NETWORK_TEMPLATE).arg(0).arg(1);
        m_mask = 16;
        m_vlanId = 3;

        m_settings->creationOut(VN_S(VMTDVInterface) + " | Constructor finished");
    }

    VMTDVInterface::~VMTDVInterface()
    {
        m_settings->creationOut(VN_S(VMTDVInterface) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDVInterface) + " | Destructor finished");
    }

    QString VMTDVInterface::toString() const
    {
        auto str = QString("Index: %1\n"
                           "Group Index: %2\n"
                           "Address: %3/%4\n"
                           "VLAN ID: %5")
                   .arg(m_index)
                   .arg(m_groupIndex)
                   .arg(m_ip).arg(m_mask)
                   .arg(m_vlanId);

        return str;
    }

    int VMTDVInterface::index()
    {
        return m_index;
    }
    void VMTDVInterface::setIndex(int index)
    {
        m_index = index;
    }

    int VMTDVInterface::groupIndex()
    {
        return m_groupIndex;
    }
    void VMTDVInterface::setGroupIndex(int groupIndex)
    {
        m_groupIndex = groupIndex;
    }

    QString VMTDVInterface::name() const
    {
        return QString("%1 [VLAN ID: %2]")
               .arg(m_ip)
               .arg(m_vlanId);
    }

    QString VMTDVInterface::interfaceName() const
    {
        return m_interfaceName;
    }

    QString VMTDVInterface::ip() const
    {
        return m_ip;
    }
    void VMTDVInterface::setIp(const QString &ip)
    {
        m_ip = ip;
    }

    uchar VMTDVInterface::mask() const
    {
        return m_mask;
    }
    void VMTDVInterface::setMask(uchar mask)
    {
        m_mask = mask;
    }

    int VMTDVInterface::vlanId() const
    {
        return m_vlanId;
    }
    void VMTDVInterface::setVlanId(int vlanId)
    {
        m_vlanId = vlanId;
    }

    const QList<VMTDVRoute> &VMTDVInterface::routes() const
    {
        return m_routes;
    }
    void VMTDVInterface::clearRoutes()
    {
        m_routes.clear();
    }
    void VMTDVInterface::addRoute(const QString &network, uchar mask,
                                  const QString &gateway, int metric)
    {
        m_routes.append(VMTDVRoute(m_settings,
                                   network, mask,
                                   gateway, metric));
    }
}

#include "VMTDParticipant.h"
#include "VMTDParticipantForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDParticipant::VMTDParticipant(QObject *parent, VMTDNodeDevice *nodeDevice)
        : QObject(parent)
        , m_nodeDevice(nodeDevice)
        , m_settings(nodeDevice->settings())
    {
        m_settings->creationOut(VN_S(VMTDParticipant) + " | Constructor called");

        m_index_1 = -1;
        m_index_2 = -1;

        m_role = EnRole::ENDPOINT;

        m_groupIndex_1 = -1;
        m_ip_1 = "192.168.250.250";
        m_mask_1 = "255.255.255.0";
        m_vlanId_1 = 1;

        m_groupIndex_2 = -1;
        m_ip_2 = "192.168.251.250";
        m_mask_2 = "255.255.255.0";
        m_vlanId_1 = 2;

        m_gateway = "192.168.250.1";

        connect(this, &VMTDParticipant::appendRequestsSignal,
                m_nodeDevice, &VMTDNodeDevice::appendRequestsSignal);

        m_settings->creationOut(VN_S(VMTDParticipant) + " | Constructor finished");
    }

    VMTDParticipant::~VMTDParticipant()
    {
        m_settings->creationOut(VN_S(VMTDParticipant) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDParticipant) + " | Destructor finished");
    }

    VMTDSettings *VMTDParticipant::settings() const
    {
        return m_settings;
    }


    const QString &VMTDParticipant::enRoleToS(const EnRole &role)
    {
        switch (role)
        {
        // *INDENT-OFF*
        case EnRole::ENDPOINT: RETURN_S("Endpoint");
        case EnRole::GATEWAY : RETURN_S("Gateway");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDParticipant::enRoleToL()
    {
        RETURN_MAP(EnRole, enRoleToS);
    }

    void VMTDParticipant::setup()
    {
        emit appendRequestsSignal(buildRequests());
    }

    int VMTDParticipant::index_1()
    {
        return m_index_1;
    }
    void VMTDParticipant::setIndex_1(int index_1)
    {
        m_index_1 = index_1;
    }

    int VMTDParticipant::index_2()
    {
        return m_index_2;
    }
    void VMTDParticipant::setIndex_2(int index_2)
    {
        m_index_2 = index_2;
    }

    VMTDNodeDevice *VMTDParticipant::nodeDevice() const
    {
        return m_nodeDevice;
    }

    VMTDParticipant::EnRole VMTDParticipant::role() const
    {
        return m_role;
    }
    void VMTDParticipant::setRole(VMTDParticipant::EnRole role)
    {
        m_role = role;
    }

    QString VMTDParticipant::name_1() const
    {
        return QString("%1 [VLAN ID: %2]")
               .arg(m_ip_1)
               .arg(m_vlanId_1);
    }

    int VMTDParticipant::groupIndex_1()
    {
        return m_groupIndex_1;
    }
    void VMTDParticipant::setGroupIndex_1(int groupIndex_1)
    {
        m_groupIndex_1 = groupIndex_1;
    }

    QString VMTDParticipant::ip_1() const
    {
        return m_ip_1;
    }
    void VMTDParticipant::setIp_1(const QString &ip_1)
    {
        m_ip_1 = ip_1;
    }

    QString VMTDParticipant::mask_1() const
    {
        return m_mask_1;
    }
    void VMTDParticipant::setMask_1(const QString &mask_1)
    {
        m_mask_1 = mask_1;
    }

    int VMTDParticipant::vlanId_1() const
    {
        return m_vlanId_1;
    }
    void VMTDParticipant::setVlanId_1(int vlanId_1)
    {
        m_vlanId_1 = vlanId_1;
    }

    QString VMTDParticipant::name_2() const
    {
        return QString("%1 [VLAN ID: %2]")
               .arg(m_ip_2)
               .arg(m_vlanId_2);
    }

    int VMTDParticipant::groupIndex_2()
    {
        return m_groupIndex_2;
    }
    void VMTDParticipant::setGroupIndex_2(int groupIndex_2)
    {
        m_groupIndex_2 = groupIndex_2;
    }

    QString VMTDParticipant::ip_2() const
    {
        return m_ip_2;
    }
    void VMTDParticipant::setIp_2(const QString &ip_2)
    {
        m_ip_2 = ip_2;
    }

    QString VMTDParticipant::mask_2() const
    {
        return m_mask_2;
    }
    void VMTDParticipant::setMask_2(const QString &mask_2)
    {
        m_mask_2 = mask_2;
    }

    int VMTDParticipant::vlanId_2() const
    {
        return m_vlanId_2;
    }
    void VMTDParticipant::setVlanId_2(int vlanId_2)
    {
        m_vlanId_2 = vlanId_2;
    }

    QString VMTDParticipant::gateway() const
    {
        return m_gateway;
    }
    void VMTDParticipant::setGateway(const QString &gateway)
    {
        m_gateway = gateway;
    }

    const QStringList &VMTDParticipant::routes() const
    {
        return m_routes;
    }
    void VMTDParticipant::setRoutes(const QStringList &routes)
    {
        m_routes = routes;
    }
    void VMTDParticipant::clearRoutes()
    {
        m_routes.clear();
    }
    void VMTDParticipant::addRoute(const QString &route)
    {
        m_routes.append(route);
    }

    const QStringList &VMTDParticipant::hosts() const
    {
        return m_hosts;
    }
    void VMTDParticipant::setHosts(const QStringList &hosts)
    {
        m_hosts = hosts;
    }
    void VMTDParticipant::clearHosts()
    {
        m_hosts.clear();
    }
    void VMTDParticipant::addHost(const QString &host)
    {
        m_hosts.append(host);
    }

    void VMTDParticipant::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDParticipantForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    QList<QPair<QString, QJsonObject> > VMTDParticipant::buildRequests()
    {
        QList<QPair<QString, QJsonObject>> requests;

        if (m_role == EnRole::ENDPOINT)
        {
            if ("Interface 1")
            {
                QJsonObject jsonObj;

                jsonObj[PRM_INTERFACE] = "vmtd1";
                jsonObj[PRM_IP] = m_ip_1;
                jsonObj[PRM_MASK] = m_mask_1;
                jsonObj[PRM_GATEWAY] = m_gateway;

                requests.append(qMakePair(MTH_SETUP_INTERFACE_1, jsonObj));
            }

            if ("Interface 2")
            {
                QJsonObject jsonObj;

                requests.append(qMakePair(MTH_CLEAR_INTERFACE_2, jsonObj));
            }

            if ("Routes")
            {
                QJsonObject jsonObj;

                requests.append(qMakePair(MTH_CLEAR_ROUTES, jsonObj));
            }
        }
        else if (m_role == EnRole::GATEWAY)
        {
            if ("Interface 1")
            {
                QJsonObject jsonObj;

                jsonObj[PRM_INTERFACE] = "vmtd1";
                jsonObj[PRM_IP] = m_ip_1;
                jsonObj[PRM_MASK] = m_mask_1;
                jsonObj[PRM_VLAN_ID] = m_vlanId_1;

                requests.append(qMakePair(MTH_SETUP_INTERFACE_1_VLAN, jsonObj));
            }

            if ("Interface 2")
            {
                QJsonObject jsonObj;

                jsonObj[PRM_INTERFACE] = "vmtd2";
                jsonObj[PRM_IP] = m_ip_2;
                jsonObj[PRM_MASK] = m_mask_2;
                jsonObj[PRM_VLAN_ID] = m_vlanId_2;

                requests.append(qMakePair(MTH_SETUP_INTERFACE_2_VLAN, jsonObj));
            }

            if ("Routes")
            {
                QJsonObject jsonObj;
                QJsonArray jsonArr;

                for (const auto &route : m_routes)
                {
                    QJsonObject routeObj;

                    const auto splittedRoute = route.split(' ');

                    routeObj[PRM_NETWORK] = splittedRoute[0];
                    routeObj[PRM_MASK] = splittedRoute[1];
                    routeObj[PRM_GATEWAY] = splittedRoute[2];

                    jsonArr.append(routeObj);
                }

                jsonObj[PRM_ROUTES] = jsonArr;

                requests.append(qMakePair(MTH_SETUP_ROUTES, jsonObj));
            }
        }

        if ("Hosts")
        {
            QJsonObject jsonObj;
            QJsonArray jsonArr;

            for (const auto &host : m_hosts)
            {
                QJsonObject hostObj;

                const auto splittedRoute = host.split(' ');

                hostObj[PRM_IP] = splittedRoute[0];
                hostObj[PRM_DOMAIN_NAME] = splittedRoute[1];

                jsonArr.append(hostObj);
            }

            jsonObj[PRM_HOSTS] = jsonArr;

            requests.append(qMakePair(MTH_SETUP_HOSTS, jsonObj));
        }

        return requests;
    }
}

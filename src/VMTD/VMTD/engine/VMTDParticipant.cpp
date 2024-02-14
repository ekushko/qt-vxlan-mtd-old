#include "VMTDParticipant.h"
#include "VMTDParticipantForm.h"

#include "../VMTDRepo.h"

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
}

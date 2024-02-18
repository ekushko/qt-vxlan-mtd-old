#include "VMTDNodeDevice.h"
#include "VMTDNodeDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeDevice::VMTDNodeDevice(QObject *parent, VMTDSettings *settings, int id)
        : VMTDDevice(parent, settings, EnType::NODE, id)
    {
        m_settings->creationOut(VN_S(VMTDNodeDevice) + " | Constructor called");

        m_ip = "127.0.0.1";
        m_role = EnRole::ENDPOINT;

        m_settings->creationOut(VN_S(VMTDNodeDevice) + " | Constructor finished");
    }

    VMTDNodeDevice::~VMTDNodeDevice()
    {
        m_settings->creationOut(VN_S(VMTDNodeDevice) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDNodeDevice) + " | Destructor finished");
    }

    const QString &VMTDNodeDevice::enRoleToS(const EnRole &role)
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
    const QMap<int, QString> &VMTDNodeDevice::enRoleToL()
    {
        RETURN_MAP(EnRole, enRoleToS);
    }

    QJsonObject VMTDNodeDevice::toJson() const
    {
        auto jsonObj = VMTDDevice::toJson();

        jsonObj[VN_ME(m_ip)] = m_ip;
        jsonObj[VN_ME(m_role)] = (int)m_role;
        jsonObj[VN_ME(m_domainName)] = m_domainName;

        return jsonObj;
    }
    void VMTDNodeDevice::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        VMTDDevice::fromJson(jsonObj);

        m_ip = jsonObj[VN_ME(m_ip)].toString(m_ip);
        m_role = (EnRole)jsonObj[VN_ME(m_role)].toInt((int)m_role);
        m_domainName = jsonObj[VN_ME(m_domainName)].toString(m_domainName);
    }

    QString VMTDNodeDevice::name() const
    {
        return QString("id: %1 [%2]")
               .arg(id())
               .arg(ip());
    }

    QString VMTDNodeDevice::ip() const
    {
        return m_ip;
    }
    void VMTDNodeDevice::setIp(const QString &ip)
    {
        m_ip = ip;
    }

    VMTDNodeDevice::EnRole VMTDNodeDevice::role() const
    {
        return m_role;
    }
    void VMTDNodeDevice::setRole(EnRole role)
    {
        m_role = role;
    }

    QString VMTDNodeDevice::domainName() const
    {
        return m_domainName;
    }
    void VMTDNodeDevice::setDomainName(const QString &domainName)
    {
        m_domainName = domainName;
    }

    void VMTDNodeDevice::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeDeviceForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }
}

#include "VMTDNodeDevice.h"
#include "VMTDNodeDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeDevice::VMTDNodeDevice(QObject *parent, VMTDSettings *settings, int id)
        : QObject(parent)
        , m_settings(settings)
        , m_id(id)
    {
        m_interfaces = new VMTDInterfaces(this, m_settings);
        m_interfaces->setOnlyOneMode(true);
    }

    VMTDNodeDevice::~VMTDNodeDevice()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
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
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_id)] = m_id;
        jsonObj[VN_ME(m_ip)] = m_ip;
        jsonObj[VN_ME(m_role)] = (int)m_role;
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;
        jsonObj[VN_ME(m_checkQueueInterval)] = m_checkQueueInterval;
        jsonObj[VN_ME(m_interfaces)] = m_interfaces->toJson();

        return jsonObj;
    }
    void VMTDNodeDevice::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_id = jsonObj[VN_ME(m_id)].toInt(m_id);
        m_ip = jsonObj[VN_ME(m_ip)].toString();
        m_role = (EnRole)jsonObj[VN_ME(m_role)].toInt();
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);
        m_checkQueueInterval = jsonObj[VN_ME(m_checkQueueInterval)]
                               .toInt(m_checkQueueInterval);
        m_interfaces->fromJson(jsonObj[VN_ME(m_interfaces)].toObject());
    }

    bool VMTDNodeDevice::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDNodeDevice::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    int VMTDNodeDevice::id() const
    {
        return m_id;
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

    int VMTDNodeDevice::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDNodeDevice::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        m_ticketTimeoutInterval = ticketTimeoutInterval;
    }

    int VMTDNodeDevice::checkQueueInterval() const
    {
        return m_checkQueueInterval;
    }
    void VMTDNodeDevice::setCheckQueueInterval(int checkQueueInterval)
    {
        m_checkQueueInterval = checkQueueInterval;
    }

    VMTDInterfaces *VMTDNodeDevice::interfaces() const
    {
        return m_interfaces;
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

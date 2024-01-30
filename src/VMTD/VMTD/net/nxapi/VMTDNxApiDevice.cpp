#include "VMTDNxApiDevice.h"
#include "VMTDNxApiDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNxApiDevice::VMTDNxApiDevice(QObject *parent, VMTDSettings *settings, int id)
        : QObject(parent)
        , m_settings(settings)
        , m_id(id)
    {
        m_interfaces = new VMTDInterfaces(this, m_settings);
        m_interfaces->setOnlyOneMode(false);

        m_url.setUserName(QString());
        m_url.setPassword(QString());
    }

    VMTDNxApiDevice::~VMTDNxApiDevice()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    QJsonObject VMTDNxApiDevice::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_id)] = m_id;
        jsonObj[VN_ME(m_url)] = m_url.toString(QUrl::RemoveUserInfo);
        jsonObj[VN_MT_REF(m_url.userName())] = m_url.userName();
        jsonObj[VN_MT_REF(m_url.password())] = m_url.password();
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;
        jsonObj[VN_ME(m_interfaces)] = m_interfaces->toJson();

        return jsonObj;
    }
    void VMTDNxApiDevice::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_id = jsonObj[VN_ME(m_id)].toInt(m_id);
        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);
        m_interfaces->fromJson(jsonObj[VN_ME(m_interfaces)].toObject());
    }

    bool VMTDNxApiDevice::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDNxApiDevice::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    int VMTDNxApiDevice::id() const
    {
        return m_id;
    }

    QUrl VMTDNxApiDevice::url() const
    {
        return m_url;
    }
    void VMTDNxApiDevice::setUrl(const QUrl &url)
    {
        m_url = url;
    }

    int VMTDNxApiDevice::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDNxApiDevice::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        m_ticketTimeoutInterval = ticketTimeoutInterval;
    }

    VMTDInterfaces *VMTDNxApiDevice::interfaces() const
    {
        return m_interfaces;
    }

    void VMTDNxApiDevice::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiDeviceForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }
}

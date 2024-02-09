#include "VMTDNxApiDevice.h"
#include "VMTDNxApiDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNxApiDevice::VMTDNxApiDevice(QObject *parent, VMTDSettings *settings, int id)
        : VMTDDevice(parent, settings, EnType::NX_API, id)
    {
        m_settings->debugOut(VN_S(VMTDNxApiDevice) + " | Constructor called");

        m_url.setUserName(QString());
        m_url.setPassword(QString());

        m_settings->debugOut(VN_S(VMTDNxApiDevice) + " | Constructor finished");
    }

    VMTDNxApiDevice::~VMTDNxApiDevice()
    {
        m_settings->debugOut(VN_S(VMTDNxApiDevice) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->debugOut(VN_S(VMTDNxApiDevice) + " | Destructor finished");
    }

    QJsonObject VMTDNxApiDevice::toJson() const
    {
        auto jsonObj = VMTDDevice::toJson();

        jsonObj[VN_ME(m_url)] = m_url.toString(QUrl::RemoveUserInfo);
        jsonObj[VN_MT_REF(m_url.userName())] = m_url.userName();
        jsonObj[VN_MT_REF(m_url.password())] = m_url.password();

        return jsonObj;
    }
    void VMTDNxApiDevice::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        VMTDDevice::fromJson(jsonObj);

        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
    }

    QString VMTDNxApiDevice::name() const
    {
        return QString("id: %1 [%2]")
               .arg(id())
               .arg(url().toString(QUrl::RemoveUserInfo));
    }

    QUrl VMTDNxApiDevice::url() const
    {
        return m_url;
    }
    void VMTDNxApiDevice::setUrl(const QUrl &url)
    {
        m_url = url;
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

#include "VMTDDevice.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDDevice::VMTDDevice(QObject *parent, VMTDSettings *settings, EnType type, int id)
        : QObject(parent)
        , m_settings(settings)
        , m_type(type)
        , m_id(id)
    {
        m_interfaceManager = new VMTDInterfaceManager(this, m_settings);
        m_interfaceManager->setOnlyOneMode(m_type == EnType::NODE);
    }

    VMTDSettings *VMTDDevice::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDDevice::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_type)] = (int)m_type;
        jsonObj[VN_ME(m_id)] = m_id;

        jsonObj[VN_ME(m_interfaceManager)] = m_interfaceManager->toJson();

        return jsonObj;
    }
    void VMTDDevice::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_id = jsonObj[VN_ME(m_id)].toInt(m_id);

        m_interfaceManager->fromJson(jsonObj[VN_ME(m_interfaceManager)].toObject());
    }

    bool VMTDDevice::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDDevice::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    VMTDDevice::EnType VMTDDevice::type() const
    {
        return m_type;
    }

    int VMTDDevice::id() const
    {
        return m_id;
    }

    VMTDInterfaceManager *VMTDDevice::interfaceManager() const
    {
        return m_interfaceManager;
    }
}

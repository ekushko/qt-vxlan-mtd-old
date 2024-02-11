#include "VMTDInterface.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDInterface::VMTDInterface(QObject *parent, VMTDSettings *settings, int id)
        : QObject(parent)
        , m_settings(settings)
        , m_id(id)
    {
        m_settings->creationOut(VN_S(VMTDInterface) + " | Constructor called");

        m_name = QString("eth%1").arg(m_id);
        m_ip = "127.0.0.1";

        m_settings->creationOut(VN_S(VMTDInterface) + " | Constructor finished");
    }

    VMTDInterface::~VMTDInterface()
    {
        m_settings->creationOut(VN_S(VMTDInterface) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDInterface) + " | Destructor finished");
    }

    VMTDSettings *VMTDInterface::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDInterface::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_id)] = m_id;
        jsonObj[VN_ME(m_name)] = m_name;
        jsonObj[VN_ME(m_ip)] = m_ip;

        return jsonObj;
    }
    void VMTDInterface::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_id = jsonObj[VN_ME(m_id)].toInt(m_id);
        m_name = jsonObj[VN_ME(m_name)].toString();
        m_ip = jsonObj[VN_ME(m_ip)].toString();
    }

    int VMTDInterface::id() const
    {
        return m_id;
    }

    QString VMTDInterface::name() const
    {
        return m_name;
    }
    void VMTDInterface::setName(const QString &name)
    {
        m_name = name;
    }

    QString VMTDInterface::ip() const
    {
        return m_ip;
    }
    void VMTDInterface::setIp(const QString &ip)
    {
        m_ip = ip;
    }
}

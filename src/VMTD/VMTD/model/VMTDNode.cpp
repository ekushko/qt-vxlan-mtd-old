#include "VMTDNode.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNode::VMTDNode(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }

    const QString &VMTDNode::enRoleToS(const EnRole &role)
    {
        switch(role)
        {
        case EnRole::ENDPOINT: RETURN_S("Endpoint");
        case EnRole::GATEWAY : RETURN_S("Gateway");
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDNode::enRoleToL()
    {
        RETURN_MAP(EnRole, enRoleToS);
    }

    QJsonObject VMTDNode::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_identificator)] = m_identificator;
        jsonObj[VN_ME(m_ip)] = m_ip;
        jsonObj[VN_ME(m_role)] = (int)m_role;
        jsonObj[VN_ME(m_portNumber)] = m_portNumber;
        jsonObj[VN_ME(m_currentSwitch)] = m_currentSwitch;

        return jsonObj;
    }
    void VMTDNode::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_identificator = jsonObj[VN_ME(m_identificator)].toInt(-1);
        m_ip = jsonObj[VN_ME(m_ip)].toString();
        m_role = (EnRole)jsonObj[VN_ME(m_role)].toInt();
        m_portNumber = jsonObj[VN_ME(m_portNumber)].toInt();
        m_currentSwitch = jsonObj[VN_ME(m_currentSwitch)].toInt(-1);
    }

    bool VMTDNode::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDNode::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    bool VMTDNode::isConnected() const
    {
        return m_currentSwitch >= 0;
    }

    int VMTDNode::identificator() const
    {
        return m_identificator;
    }
    void VMTDNode::setIdentificator(int identificator)
    {
        m_identificator = identificator;
    }

    QString VMTDNode::ip() const
    {
        return m_ip;
    }
    void VMTDNode::setIp(const QString &ip)
    {
        m_ip = ip;
    }

    VMTDNode::EnRole VMTDNode::role() const
    {
        return m_role;
    }
    void VMTDNode::setRole(EnRole role)
    {
        m_role = role;
    }

    int VMTDNode::portNumber() const
    {
        return m_portNumber;
    }
    void VMTDNode::setPortNumber(int portNumber)
    {
        m_portNumber = portNumber;
    }

    int VMTDNode::currentSwitch() const
    {
        return m_currentSwitch;
    }
    void VMTDNode::setCurrentSwitch(int currentSwitch)
    {
        m_currentSwitch = currentSwitch;
    }
}

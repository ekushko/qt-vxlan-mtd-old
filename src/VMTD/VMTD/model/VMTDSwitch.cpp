#include "VMTDSwitch.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDSwitch::VMTDSwitch(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }

    QJsonObject VMTDSwitch::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_identificator)] = m_identificator;
        jsonObj[VN_ME(m_url)] = m_url.toString();
        jsonObj[VN_MT_REF(m_url.userName())] = m_url.userName();
        jsonObj[VN_MT_REF(m_url.password())] = m_url.password();
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;


        jsonObj[VN_ME(m_portCount)] = m_portCount;

        QJsonObject portsObj;

        for (auto portNumber : PortToNode)
            portsObj[QString("port_%1").arg(portNumber)] = PortToNode.at(portNumber);

        jsonObj["ports"] = portsObj;

        return jsonObj;
    }
    void VMTDSwitch::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_identificator = jsonObj[VN_ME(m_identificator)].toInt();
        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);


        m_portCount = 0;

        PortToNode.clear();
        setPortCount(jsonObj[VN_ME(m_portCount)].toInt());

        auto portsObj = jsonObj["ports"].toObject();

        for (int i = 0; i < m_portCount; ++i)
            PortToNode[i] = portsObj[QString("port_%1").arg(i)].toInt(-1);
    }

    bool VMTDSwitch::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDSwitch::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    int VMTDSwitch::identificator() const
    {
        return m_identificator;
    }
    void VMTDSwitch::setIdentificator(int identificator)
    {
        m_identificator = identificator;
    }

    QUrl VMTDSwitch::url() const
    {
        return m_url;
    }
    void VMTDSwitch::setUrl(const QUrl &url)
    {
        m_url = url;
    }

    int VMTDSwitch::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDSwitch::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        m_ticketTimeoutInterval = ticketTimeoutInterval;
    }

    int VMTDSwitch::portCount() const
    {
        return m_portCount;
    }
    void VMTDSwitch::setPortCount(int portCount)
    {
        if (m_portCount != portCount)
        {
            PortToNode.resize(portCount);

            if (portCount > m_portCount)
            {
                for (int i = m_portCount; i < PortToNode.size(); ++i)
                    PortToNode[i] = -1;
            }

            m_portCount = portCount;
        }
    }
}

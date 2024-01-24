#include "VMTDSwitch.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDSwitch::VMTDSwitch(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_url.setUserName(QString());
        m_url.setPassword(QString());

        PortToNode = QVector<int>(m_portCount, -1);

        PortToInterface = QVector<QString>(m_portCount, QString());
    }

    QJsonObject VMTDSwitch::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_id)] = m_id;
        jsonObj[VN_ME(m_url)] = m_url.toString(QUrl::RemoveUserInfo);
        jsonObj[VN_MT_REF(m_url.userName())] = m_url.userName();
        jsonObj[VN_MT_REF(m_url.password())] = m_url.password();
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;


        jsonObj[VN_ME(m_portCount)] = m_portCount;

        QJsonObject portsObj;

        for (int i = 0; i < PortToNode.size(); ++i)
            portsObj[QString("port_%1").arg(i)] = PortToNode.at(i);

        jsonObj[VN_S(PortToNode)] = portsObj;


        QJsonObject interfacesObj;

        for (int i = 0; i < PortToInterface.size(); ++i)
            interfacesObj[QString("interface_%1").arg(i)] = PortToInterface.at(i);

        jsonObj[VN_S(PortToInterface)] = interfacesObj;


        return jsonObj;
    }
    void VMTDSwitch::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_id = jsonObj[VN_ME(m_id)].toInt();
        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);


        m_portCount = 0;

        PortToNode.clear();
        setPortCount(jsonObj[VN_ME(m_portCount)].toInt());

        auto portsObj = jsonObj[VN_S(PortToNode)].toObject();

        for (int i = 0; i < m_portCount; ++i)
            PortToNode[i] = portsObj[QString("port_%1").arg(i)].toInt(-1);

        auto interfacesObj = jsonObj[VN_S(PortToInterface)].toObject();

        for (int i = 0; i < m_portCount; ++i)
            PortToInterface[i] = interfacesObj[QString("interface_%1").arg(i)].toString();
    }

    bool VMTDSwitch::isOnline() const
    {
        return m_isOnline;
    }
    void VMTDSwitch::setOnline(bool isOnline)
    {
        m_isOnline = isOnline;
    }

    int VMTDSwitch::id() const
    {
        return m_id;
    }
    void VMTDSwitch::setId(int id)
    {
        m_id = id;
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
            PortToInterface.resize(portCount);

            if (portCount > m_portCount)
            {
                for (int i = m_portCount; i < PortToNode.size(); ++i)
                    PortToNode[i] = -1;

                for (int i = m_portCount; i < PortToInterface.size(); ++i)
                    PortToInterface[i] = QString();
            }

            m_portCount = portCount;

            emit portCountChangedSignal();
        }
    }
}

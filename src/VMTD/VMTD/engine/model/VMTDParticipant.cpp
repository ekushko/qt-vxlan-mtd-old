#include "VMTDParticipant.h"
#include "VMTDParticipantForm.h"

#include "../../VMTDRepo.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDParticipant::VMTDParticipant(QObject *parent, VMTDNodeDevice *nodeDevice)
        : QObject(parent)
        , m_nodeDevice(nodeDevice)
        , m_settings(nodeDevice->settings())
    {
        m_settings->creationOut(VN_S(VMTDParticipant) + " | Constructor called");

        m_role = EnRole::ENDPOINT;

        QString interfaceName;

        if (m_nodeDevice->interfaceManager()->interfaces().size() > 0)
            interfaceName = m_nodeDevice->interfaceManager()->interfaces().first()->name();

        m_vInterface1 = new VMTDVInterface(this, m_settings, interfaceName);
        m_vInterface2 = new VMTDVInterface(this, m_settings, interfaceName);

        connect(this, &VMTDParticipant::appendRequestsSignal,
                m_nodeDevice, &VMTDNodeDevice::appendRequestsSignal);

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

    void VMTDParticipant::setup()
    {
        emit appendRequestsSignal(buildRequests());
    }

    void VMTDParticipant::checkConnection()
    {
        QList<QPair<QString, QJsonObject>> requests;

        requests.append(qMakePair(MTH_CHECK_CONNECTION, QJsonObject()));

        emit appendRequestsSignal(requests);
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

    VMTDVInterface *VMTDParticipant::vInterface1() const
    {
        return m_vInterface1;
    }

    VMTDVInterface *VMTDParticipant::vInterface2() const
    {
        return m_vInterface2;
    }

    const QStringList &VMTDParticipant::hosts() const
    {
        return m_hosts;
    }
    void VMTDParticipant::setHosts(const QStringList &hosts)
    {
        m_hosts = hosts;
    }
    void VMTDParticipant::clearHosts()
    {
        m_hosts.clear();
    }
    void VMTDParticipant::addHost(const QString &host)
    {
        m_hosts.append(host);
    }

    void VMTDParticipant::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDParticipantForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    QList<QPair<QString, QJsonObject>> VMTDParticipant::buildRequests()
    {
        QList<QPair<QString, QJsonObject>> requests;

        // INTERFACE 1
        {
            QJsonObject jsonObj;

            if (m_role == EnRole::ENDPOINT
                || m_role == EnRole::GATEWAY)
            {
                jsonObj[PRM_INTERFACE] = m_vInterface1->interfaceName();
                jsonObj[PRM_IP] = m_vInterface1->ip();
                jsonObj[PRM_MASK] = m_vInterface1->mask();

                QJsonArray jsonArr;

                for (const auto &route : m_vInterface1->routes())
                {
                    QJsonObject routeObj;

                    routeObj[PRM_NETWORK] = route.network();
                    routeObj[PRM_MASK] = route.mask();
                    routeObj[PRM_GATEWAY] = route.gateway();
                    routeObj[PRM_METRIC] = route.metric();

                    jsonArr.append(routeObj);
                }

                jsonObj[PRM_ROUTES] = jsonArr;

                requests.append(qMakePair(MTH_SETUP_INTERFACE_1, jsonObj));
            }
            else
            {
                requests.append(qMakePair(MTH_CLEAR_INTERFACE_1, jsonObj));
            }
        }

        // INTERFACE 2
        {
            QJsonObject jsonObj;

            if (m_role == EnRole::GATEWAY)
            {
                jsonObj[PRM_INTERFACE] = m_vInterface2->interfaceName();
                jsonObj[PRM_IP] = m_vInterface2->ip();
                jsonObj[PRM_MASK] = m_vInterface2->mask();

                QJsonArray jsonArr;

                for (const auto &route : m_vInterface2->routes())
                {
                    QJsonObject routeObj;

                    routeObj[PRM_NETWORK] = route.network();
                    routeObj[PRM_MASK] = route.mask();
                    routeObj[PRM_GATEWAY] = route.gateway();
                    routeObj[PRM_METRIC] = route.metric();

                    jsonArr.append(routeObj);
                }

                jsonObj[PRM_ROUTES] = jsonArr;

                requests.append(qMakePair(MTH_SETUP_INTERFACE_2, jsonObj));
            }
            else
            {
                requests.append(qMakePair(MTH_CLEAR_INTERFACE_2, jsonObj));
            }
        }

        // HOSTS
        {
            QJsonObject jsonObj;
            QJsonArray jsonArr;

            for (const auto &host : m_hosts)
            {
                QJsonObject hostObj;

                const auto splittedRoute = host.split(' ');

                hostObj[PRM_IP] = splittedRoute[0];
                hostObj[PRM_DOMAIN_NAME] = splittedRoute[1];

                jsonArr.append(hostObj);
            }

            jsonObj[PRM_HOSTS] = jsonArr;

            requests.append(qMakePair(MTH_SETUP_HOSTS, jsonObj));
        }

        return requests;
    }
}

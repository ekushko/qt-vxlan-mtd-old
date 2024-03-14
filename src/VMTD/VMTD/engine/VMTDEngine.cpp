#include "VMTDEngine.h"
#include "VMTDEngineForm.h"

#include "../VMTDRepo.h"

#include <random>
#include <algorithm>

namespace VMTDLib
{
    VMTDEngine::VMTDEngine(QObject *parent,
                           VMTDDeviceManager *deviceManager,
                           VMTDConnectionManager *connectionManager,
                           VMTDProtocol *protocol)
        : QObject(parent)
        , m_deviceManager(deviceManager)
        , m_connectionManager(connectionManager)
        , m_protocol(protocol)
        , m_settings(protocol->settings())
    {
        m_settings->creationOut(VN_S(VMTDEngine) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDEngine) + " | Constructor finished");
    }

    VMTDEngine::~VMTDEngine()
    {
        m_settings->creationOut(VN_S(VMTDEngine) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDEngine) + " | Destructor finished");
    }

    VMTDSettings *VMTDEngine::settings() const
    {
        return m_settings;
    }

    const QVector<VMTDBridge *> &VMTDEngine::bridges() const
    {
        return m_bridges;
    }

    const QVector<VMTDParticipant *> &VMTDEngine::participants() const
    {
        return m_participants;
    }

    const QVector<VMTDGroup *> &VMTDEngine::groups() const
    {
        return m_groups;
    }

    const QVector<VMTDParticipant *> &VMTDEngine::gateways() const
    {
        return m_gateways;
    }

    void VMTDEngine::startSlot()
    {
        createParticipantsAndBridges();
    }

    void VMTDEngine::mixSlot()
    {
        clearAll();

        if (m_participants.size() < 2)
        {
            m_settings->debugOut(VN_S(VMTDEngine) + " | Count of participants is not enough!");
        }
        else
        {
            createGroups();
            createGateways();
            createRoutes();
            createHosts();
            setup();
        }

        emit mixedSignal();
    }

    void VMTDEngine::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDEngineForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDEngine::clearAll()
    {
        m_gateways.clear();

        qDeleteAll(m_groups);
        m_groups.clear();
    }

    void VMTDEngine::createParticipantsAndBridges()
    {
        const auto &nxApiDevices = m_deviceManager->nxApiDevices();

        for (auto it = nxApiDevices.begin(); it != nxApiDevices.end(); ++it)
        {
            auto nxApiDevice = it.value();

            if (m_settings->shouldCheckOnline() && !nxApiDevice->isOnline())
            {
                m_settings->debugOut(QString("%1 | Device %2 is offline!")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(nxApiDevice->name()));
                continue;
            }

            const auto &interfaces = it.value()->interfaceManager()->interfaces();

            for (auto jt = interfaces.begin(); jt != interfaces.end(); ++jt)
            {
                auto interface = jt.value();

                m_settings->debugOut(QString("%1 | Finding connections for device %2 interface id: %3 [%4]...")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(nxApiDevice->name())
                                     .arg(interface->id())
                                     .arg(interface->name()));

                auto pair = m_connectionManager->connectedDeviceInterface(nxApiDevice, interface);

                auto connectedDevice = pair.first;
                auto connectedInterface = pair.second;

                if (connectedDevice == nullptr)
                {
                    continue;
                }
                else if (connectedDevice->type() == VMTDDevice::EnType::NX_API)
                {
                    auto bridge = new VMTDBridge(this, m_settings);
                    bridge->set(nxApiDevice, interface,
                                dynamic_cast<VMTDNxApiDevice *>(connectedDevice), connectedInterface);
                    m_bridges.append(bridge);
                }
                else if (connectedDevice->type() == VMTDDevice::EnType::NODE)
                {
                    if (m_settings->shouldCheckOnline() && !connectedDevice->isOnline())
                    {
                        m_settings->debugOut(QString("%1 | Node is offline! Device %2 interface id: %3 [%4]")
                                             .arg(VN_S(VMTDEngine))
                                             .arg(connectedDevice->name())
                                             .arg(connectedInterface->id())
                                             .arg(connectedInterface->name()));
                        continue;
                    }

                    auto participant = new VMTDParticipant(this, dynamic_cast<VMTDNodeDevice *>(connectedDevice));
                    participant->vInterface1()->setIndex(m_participants.size());
                    participant->setRole(VMTDParticipant::EnRole::ENDPOINT);
                    m_participants.append(participant);
                }
            }
        }
    }

    void VMTDEngine::createGroups()
    {
        const int minGroupCount = 2, maxGroupCount = m_participants.size();
        const int groupCount = qrand()
                               % (maxGroupCount - minGroupCount + 1) + minGroupCount;

        m_settings->debugOut(QString("%1 | Will be created this count of groups: %2")
                             .arg(VN_S(VMTDEngine))
                             .arg(groupCount));

        QSet<int> _octets;
        m_vlanIds.clear();

        for (int i = 0; i < groupCount; ++i)
        {
            auto group = new VMTDGroup(this, m_settings);

            // индекс

            group->setIndex(i);

            // vlan id

            const int minVlanId = m_settings->minVlanId(), maxVlanId = m_settings->maxVlanId();

            int vlanId = 0;

            do
            {
                vlanId = qrand()
                         % (maxVlanId - minVlanId + 1) + minVlanId;
                group->setVlanId(vlanId);
            }
            while (m_vlanIds.contains(vlanId));

            m_vlanIds.insert(vlanId);

            // сеть

            const int minThirdOctet = 2, maxThirdOctet = 254;

            int thirdOctet = 0;

            do
            {
                thirdOctet = qrand()
                             % (maxThirdOctet - minThirdOctet + 1) + minThirdOctet;
                group->setNetwork(QString(NETWORK_TEMPLATE).arg(thirdOctet).arg(0));
            }
            while (_octets.contains(thirdOctet));

            _octets.insert(thirdOctet);

            // маска

            group->setMask(24);

            // максимальное число участников

            const auto whole = m_participants.size() / groupCount;
            const auto remainder = m_participants.size() % groupCount;

            if (i == 0 && remainder != 0)
                group->setMaxParticipantCount(whole + remainder);
            else
                group->setMaxParticipantCount(whole);

            m_groups.append(group);
        }

        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        QVector<VMTDParticipant *> _participants(m_participants.begin(), m_participants.end());
        QVector<VMTDGroup *> _groups(m_groups.begin(), m_groups.end());

        std::shuffle(_participants.begin(), _participants.end(), generator);
        std::shuffle(_groups.begin(), _groups.end(), generator);

        for (int i = 0, j = 0; i < _groups.size(); ++i)
        {
            auto group = _groups.at(i);

            while (group->canAddParticipant())
            {
                auto participant = _participants.at(j++);
                participant->setRole(VMTDParticipant::EnRole::ENDPOINT);

                group->addParticipant(participant);
            }
        }
    }

    void VMTDEngine::createGateways()
    {
        for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
        {
            auto group = *it;

            const auto index = qrand() % group->participants().size();

            auto gateway = group->participants().at(index);
            gateway->vInterface2()->setIndex(m_gateways.size());
            gateway->setRole(VMTDParticipant::EnRole::GATEWAY);
            m_gateways.append(gateway);

            m_settings->debugOut(QString("%1 | Participant %2 was choosen as gateway in group %3")
                                 .arg(VN_S(VMTDEngine))
                                 .arg(gateway->vInterface1()->name())
                                 .arg(group->name()));

            for (int i = 0; i < group->participants().size(); ++i)
            {
                auto participant = group->participants().at(i);
                participant->vInterface1()->addRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                                                     gateway->vInterface1()->ip(), 100);
            }
        }
    }

    void VMTDEngine::createRoutes()
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        for (auto participant : m_participants)
        {
            participant->vInterface1()->clearRoutes();
            participant->vInterface2()->clearRoutes();
        }

        QVector<VMTDParticipant *> _gateways(m_gateways.begin(), m_gateways.end());

        std::shuffle(_gateways.begin(), _gateways.end(), generator);

        for (auto it = _gateways.begin(); it != _gateways.end(); ++it)
        {
            auto participant = *it;
            auto group = m_groups.at(participant->vInterface1()->groupIndex());

            const auto distance = std::distance(it, _gateways.end());

            if (distance <= 1)
                break;

            auto gateway = *std::next(it);
            group->setGateway(gateway);

            if (distance <= 2)
                break;

            auto jt = it;
            std::advance(jt, 2);

            while (jt != _gateways.end())
            {
                auto remoteParticipant = *jt++;
                auto remoteGroup = m_groups.at(remoteParticipant->vInterface1()->groupIndex());

                participant->vInterface2()->addRoute(remoteGroup->network(), remoteGroup->mask(),
                                                     gateway->vInterface1()->ip(), 100);

                auto group = m_groups.at(gateway->vInterface1()->groupIndex());

                m_settings->debugOut(QString("%1 | Route [%2] was added to gateway %3 in group %4")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(participant->vInterface2()->routes().last().toString())
                                     .arg(gateway->vInterface1()->name())
                                     .arg(group->name()));
            }
        }

        for (auto it = _gateways.rbegin(); it != _gateways.rend(); ++it)
        {
            auto participant = *it;
            auto group = m_groups.at(participant->vInterface1()->groupIndex());

            const auto distance = std::distance(it, _gateways.rend());

            if (distance <= 1)
                break;

            auto gateway = *std::next(it);

            if (distance <= 2)
                break;

            auto jt = it;
            std::advance(jt, 2);

            while (jt != _gateways.rend())
            {
                auto remoteParticipant = *jt++;
                auto remoteGroup = m_groups.at(remoteParticipant->vInterface1()->groupIndex());

                participant->vInterface1()->addRoute(remoteGroup->network(), remoteGroup->mask(),
                                                     gateway->vInterface1()->ip(), 100);

                m_settings->debugOut(QString("%1 | Route [%2] was added to gateway %3 in group %4")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(participant->vInterface1()->routes().last().toString())
                                     .arg(gateway->vInterface1()->name())
                                     .arg(group->name()));
            }
        }

        {
            auto firstGateway = _gateways.first();
            auto group = m_groups.at(firstGateway->vInterface1()->groupIndex());

            firstGateway->setRole(VMTDParticipant::EnRole::ENDPOINT);

            for (int i = 0; i < group->participants().size(); ++i)
            {
                auto participant = group->participants().at(i);

                participant->vInterface1()->clearRoutes();
                participant->vInterface2()->clearRoutes();

                participant->vInterface1()->addRoute(QString(NETWORK_TEMPLATE).arg(0).arg(0), 16,
                                                     group->gateway()->vInterface2()->ip(), 100);
            }
        }
    }

    void VMTDEngine::createHosts()
    {
        QStringList hosts;

        for (auto participant : m_participants)
        {
            const auto domainName = participant->nodeDevice()->domainName();

            if (domainName.isEmpty())
                continue;

            hosts.append(QString("%1 %2")
                         .arg(participant->vInterface1()->ip())
                         .arg(domainName));

            if (participant->role() == VMTDParticipant::EnRole::GATEWAY)
            {
                hosts.append(QString("%1 %2")
                             .arg(participant->vInterface2()->ip())
                             .arg(domainName));
            }
        }

        for (auto participant : m_participants)
            participant->setHosts(hosts);
    }

    void VMTDEngine::setup()
    {
        for (auto participant : m_participants)
            participant->setup();
    }
}

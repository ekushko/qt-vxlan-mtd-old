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

        createGroups();
        createGateways();
        createRoutes();

        emit mixedSignal();
    }

    void VMTDEngine::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDEngineForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDEngine::clearAll()
    {
        qDeleteAll(m_gateways);
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

//            if (!nxApiDevice->isOnline())
//            {
//                m_settings->debugOut(QString("%1 | Device %2 is offline!")
//                                     .arg(VN_S(VMTDEngine))
//                                     .arg(nxApiDevice->name()));
//                continue;
//            }

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
//                    if (!connectedDevice->isOnline())
//                    {
//                        m_settings->debugOut(QString("%1 | Node is offline! Device %2 interface id: %3 [%4]")
//                                             .arg(VN_S(VMTDEngine))
//                                             .arg(connectedDevice->name())
//                                             .arg(connectedInterface->id())
//                                             .arg(connectedInterface->name()));
//                        continue;
//                    }

                    auto participant = new VMTDParticipant(this, dynamic_cast<VMTDNodeDevice *>(connectedDevice));
                    participant->setIndex_1(m_participants.size());
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

        for (int i = 0; i < groupCount; ++i)
        {
            auto group = new VMTDGroup(this, m_settings);
            group->setIndex(i);

            if (i == 0 && groupCount % m_participants.size() != 0)
                group->setMaxParticipantCount((groupCount / 2) + (groupCount % 2));
            else
                group->setMaxParticipantCount(groupCount / 2);

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
            gateway->setIndex_2(m_gateways.size());
            gateway->setRole(VMTDParticipant::EnRole::GATEWAY);
            m_gateways.append(gateway);

            m_settings->debugOut(QString("%1 | Participant %2 was choosen as gateway in group_%3")
                                 .arg(VN_S(VMTDEngine))
                                 .arg(gateway->nodeDevice()->name())
                                 .arg(group->index()));

            for (int i = 0; i < group->participants().size(); ++i)
            {
                auto participant = group->participants().at(i);
                participant->setGateway(gateway->ip_1());
            }
        }
    }

    void VMTDEngine::createRoutes()
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        QVector<VMTDParticipant *> _gateways(m_gateways.begin(), m_gateways.end());

        std::shuffle(_gateways.begin(), _gateways.end(), generator);

        for (auto it = _gateways.begin(), jt = it; it != _gateways.end(); ++it)
        {
            auto participant = *it;

            if (std::distance(it, _gateways.end()) <= 2)
                return;

            auto gateway = *std::next(it);

            std::advance(jt, 2);

            participant->clearRoutes();

            while (jt != _gateways.end())
            {
                auto remoteParticipant = *jt;
                auto remoteGroup = m_groups.at(remoteParticipant->groupIndex_1());

                const auto route = QString("%1 %2 %3")
                                   .arg(remoteGroup->network())
                                   .arg(remoteGroup->mask())
                                   .arg(gateway->ip_1());
                participant->addRoute(route);

                m_settings->debugOut(QString("%1 | Route [%2] was added to gateway %3 in group_%4")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(route)
                                     .arg(gateway->nodeDevice()->name())
                                     .arg(gateway->groupIndex_1()));
            }
        }

        for (auto it = _gateways.rbegin(), jt = it; it != _gateways.rend(); ++it)
        {
            auto participant = *it;

            if (std::distance(it, _gateways.rend()) <= 2)
                return;

            auto gateway = *std::next(it);
            std::advance(jt, 2);

            while (jt != _gateways.rend())
            {
                auto remoteParticipant = *jt;
                auto remoteGroup = m_groups.at(remoteParticipant->groupIndex_1());

                const auto route = QString("%1 %2 %3")
                                   .arg(remoteGroup->network())
                                   .arg(remoteGroup->mask())
                                   .arg(gateway->ip_1());
                participant->addRoute(route);

                m_settings->debugOut(QString("%1 | Route [%2] was added to gateway %3 in group_%4")
                                     .arg(VN_S(VMTDEngine))
                                     .arg(route)
                                     .arg(gateway->nodeDevice()->name())
                                     .arg(gateway->groupIndex_1()));
            }
        }
    }
}

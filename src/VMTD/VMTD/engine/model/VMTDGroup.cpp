#include "VMTDGroup.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDGroup::VMTDGroup(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDGroup) + " | Constructor called");

        m_index = -1;
        m_maxParticipantCount = 2;
        m_network = QString("192.168.250.0");
        m_vlanId = 1;

        m_settings->creationOut(VN_S(VMTDGroup) + " | Constructor finished");
    }

    VMTDGroup::~VMTDGroup()
    {
        m_settings->creationOut(VN_S(VMTDGroup) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDGroup) + " | Destructor finished");
    }

    const QVector<VMTDParticipant *> &VMTDGroup::participants() const
    {
        return m_participants;
    }
    bool VMTDGroup::canAddParticipant() const
    {
        return m_participants.size() < maxParticipantCount();
    }
    void VMTDGroup::addParticipant(VMTDParticipant *participant)
    {
        if (!canAddParticipant())
        {
            m_settings->debugOut(QString("%1 | Group %2 is crowded! Participant %3 not added!")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(name())
                                 .arg(participant->name_1()));
            return;
        }

        if (m_participants.contains(participant))
        {
            m_settings->debugOut(QString("%1 | Participant %2 is already in group %3! ")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(participant->name_1())
                                 .arg(name()));
            return;
        }

        participant->setGroupIndex_1(m_index);
        participant->setIp_1(QString("%1.%2")
                             .arg(m_network.left(m_network.lastIndexOf('.')))
                             .arg(m_participants.size() + 1));
        participant->setMask_1(m_mask);
        participant->setVlanId_1(m_vlanId);
        m_participants.append(participant);

        m_settings->debugOut(QString("%1 | Participant %2 was added in group %3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(participant->name_1())
                             .arg(name()));
    }
    void VMTDGroup::clearParticipants()
    {
        m_participants.clear();
    }

    const QVector<VMTDParticipant *> &VMTDGroup::gateways() const
    {
        return m_gateways;
    }
    bool VMTDGroup::canAddGateway() const
    {
        return m_gateways.size() < maxGatewayCount();
    }
    void VMTDGroup::addGateway(VMTDParticipant *gateway)
    {
        if (!canAddGateway())
        {
            m_settings->debugOut(QString("%1 | Group %2 is crowded! Gateway %3 not added!")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(name())
                                 .arg(gateway->name_2()));
            return;
        }

        if (m_gateways.contains(gateway))
        {
            m_settings->debugOut(QString("%1 | Gateway %2 is already in group %3! ")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(gateway->name_2())
                                 .arg(name()));
            return;
        }

        gateway->setGroupIndex_2(m_index);
        gateway->setIp_2(QString("%1.%2")
                         .arg(m_network.left(m_network.lastIndexOf('.')))
                         .arg(254 - m_gateways.size()));
        gateway->setMask_2(m_mask);
        gateway->setVlanId_2(m_vlanId);
        m_gateways.append(gateway);

        m_settings->debugOut(QString("%1 | Gateway %2 was added in group %3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(gateway->name_2())
                             .arg(name()));
    }
    void VMTDGroup::clearGateways()
    {
        m_gateways.clear();
    }

    QString VMTDGroup::name() const
    {
        return QString("%1 [VLAN ID: %2]")
               .arg(m_network)
               .arg(m_vlanId);
    }

    int VMTDGroup::index() const
    {
        return m_index;
    }
    void VMTDGroup::setIndex(int index)
    {
        m_index = index;
    }

    int VMTDGroup::minParticipantCount() const
    {
        return 1;
    }
    int VMTDGroup::maxParticipantCount() const
    {
        return m_maxParticipantCount;
    }
    void VMTDGroup::setMaxParticipantCount(int maxParticipantCount)
    {
        m_maxParticipantCount = maxParticipantCount;
    }

    int VMTDGroup::minGatewayCount() const
    {
        return 1;
    }
    int VMTDGroup::maxGatewayCount() const
    {
        return 2;
    }

    QString VMTDGroup::network() const
    {
        return m_network;
    }
    void VMTDGroup::setNetwork(const QString &network)
    {
        m_network = network;
    }

    QString VMTDGroup::mask() const
    {
        return m_mask;
    }
    void VMTDGroup::setMask(const QString &mask)
    {
        m_mask = mask;
    }

    int VMTDGroup::vlanId() const
    {
        return m_vlanId;
    }
    void VMTDGroup::setVlanId(int vlanId)
    {
        m_vlanId = vlanId;
    }
}

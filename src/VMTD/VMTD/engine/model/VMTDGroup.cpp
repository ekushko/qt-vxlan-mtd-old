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
        m_mask = 24;
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
                                 .arg(participant->vInterface1()->name()));
            return;
        }

        if (m_participants.contains(participant))
        {
            m_settings->debugOut(QString("%1 | Participant %2 is already in group %3! ")
                                 .arg(VN_S(VMTDGroup))
                                 .arg(participant->vInterface1()->name())
                                 .arg(name()));
            return;
        }

        participant->vInterface1()->setGroupIndex(m_index);
        participant->vInterface1()->setIp(QString("%1.%2")
                                          .arg(m_network.left(m_network.lastIndexOf('.')))
                                          .arg(m_participants.size() + 1));
        participant->vInterface1()->setMask(m_mask);
        participant->vInterface1()->setVlanId(m_vlanId);
        m_participants.append(participant);

        m_settings->debugOut(QString("%1 | Participant %2 was added in group %3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(participant->vInterface1()->name())
                             .arg(name()));
    }
    void VMTDGroup::clearParticipants()
    {
        m_participants.clear();
    }

    VMTDParticipant *VMTDGroup::gateway() const
    {
        return m_gateway;
    }
    void VMTDGroup::setGateway(VMTDParticipant *gateway)
    {
        gateway->vInterface2()->setGroupIndex(m_index);
        gateway->vInterface2()->setIp(QString("%1.254")
                                      .arg(m_network.left(m_network.lastIndexOf('.'))));
        gateway->vInterface2()->setMask(m_mask);
        gateway->vInterface2()->setVlanId(m_vlanId);


        m_settings->debugOut(QString("%1 | Gateway %2 was added in group %3! ")
                             .arg(VN_S(VMTDGroup))
                             .arg(gateway->vInterface2()->name())
                             .arg(name()));

        m_gateway = gateway;
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

    QString VMTDGroup::network() const
    {
        return m_network;
    }
    void VMTDGroup::setNetwork(const QString &network)
    {
        m_network = network;
    }

    uchar VMTDGroup::mask() const
    {
        return m_mask;
    }
    void VMTDGroup::setMask(uchar mask)
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

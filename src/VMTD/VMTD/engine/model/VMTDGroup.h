#pragma once

#include "VMTDParticipant.h"

namespace VMTDLib
{
    class VMTD_SHARED VMTDGroup : public QObject
    {
        Q_OBJECT

    public:

        VMTDGroup(QObject *parent, VMTDSettings *settings);
        ~VMTDGroup();

        // ЛОГИКА

        const QVector<VMTDParticipant *> &participants() const;
        bool canAddParticipant() const;
        void    addParticipant(VMTDParticipant *participant);
        void  clearParticipants();

        VMTDParticipant *gateway() const;
        void          setGateway(VMTDParticipant *gateway);

        // МЕТАДАННЫЕ

        QString name() const;

        int     index() const;
        void setIndex(int index);

        int     minParticipantCount() const;
        int     maxParticipantCount() const;
        void setMaxParticipantCount(int maxParticipantCount);

        // ДАННЫЕ

        QString network() const;
        void setNetwork(const QString &network);

        uchar   mask() const;
        void setMask(uchar mask);

        int     vlanId() const;
        void setVlanId(int vlanId);

    private:

        VMTDSettings *m_settings;

        int m_index;

        int m_maxParticipantCount;

        QString m_network;
        uchar m_mask;
        int m_vlanId;

        QVector<VMTDParticipant *> m_participants;

        VMTDParticipant *m_gateway = nullptr;
    };
}

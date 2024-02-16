#pragma once

#include "../device/VMTDDeviceManager.h"
#include "../connection/VMTDConnectionManager.h"
#include "../protocol/VMTDProtocol.h"

#include "VMTDBridge.h"
#include "VMTDGroup.h"

namespace VMTDLib
{
    class VMTDEngineForm;

    class VMTDEngine : public QObject
    {
        Q_OBJECT

    public:

        VMTDEngine(QObject *parent,
                   VMTDDeviceManager *deviceManager,
                   VMTDConnectionManager *connectionManager,
                   VMTDProtocol *protocol);
        ~VMTDEngine();

        VMTDSettings *settings() const;

        // ДАННЫЕ

        const QVector<VMTDBridge *> &bridges() const;

        const QVector<VMTDParticipant *> &participants() const;

        const QVector<VMTDGroup *> &groups() const;

        const QVector<VMTDParticipant *> &gateways() const;

    signals:

        void mixedSignal();

    public slots:

        void startSlot();

        void mixSlot();

        void showFormSlot(QWidget *parent);

    private:

        void clearAll();
        void createParticipantsAndBridges();
        void createGroups();
        void createGateways();
        void createRoutes();

        QPointer<VMTDEngineForm> m_form;

        VMTDDeviceManager *m_deviceManager;
        VMTDConnectionManager *m_connectionManager;
        VMTDProtocol *m_protocol;
        VMTDSettings *m_settings;

        QVector<VMTDBridge *> m_bridges;
        QVector<VMTDParticipant *> m_participants;
        QVector<VMTDGroup *> m_groups;
        QVector<VMTDParticipant *> m_gateways;
        QSet<int> m_vlanIds;
    };
}

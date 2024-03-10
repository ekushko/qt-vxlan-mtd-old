#pragma once

#include "../../device/node/VMTDNodeDevice.h"
#include "VMTDVInterface.h"

namespace VMTDLib
{
    class VMTDParticipantForm;

    class VMTDParticipant : public QObject
    {
        Q_OBJECT

    public:

        VMTDParticipant(QObject *parent, VMTDNodeDevice *nodeDevice);
        ~VMTDParticipant();

        VMTDSettings *settings() const;

        // КОНСТАНТЫ

        enum class EnRole
        {
            ENDPOINT,
            GATEWAY
        };
        Q_ENUM(EnRole)
        static const QString            &enRoleToS(const EnRole &role);
        static const QMap<int, QString> &enRoleToL();

        // ЛОГИКА

        void setup();

        void checkConnection();

        // ДАННЫЕ

        VMTDNodeDevice *nodeDevice() const;

        EnRole  role() const;
        void setRole(EnRole role);

        VMTDVInterface *vInterface1() const;
        VMTDVInterface *vInterface2() const;

        const QStringList &hosts() const;
        void            setHosts(const QStringList &hosts);
        void          clearHosts();
        void            addHost(const QString &host);

    signals:

        void appendRequestsSignal(const QList<QPair<QString, QJsonObject>> &requests);

    public slots:

        void showFormSlot();

    private:

        QList<QPair<QString, QJsonObject>> buildRequests();

        QPointer<VMTDParticipantForm> m_form;

        VMTDNodeDevice *m_nodeDevice;

        VMTDSettings *m_settings;

        EnRole m_role;

        VMTDVInterface *m_vInterface1;
        VMTDVInterface *m_vInterface2;

        QStringList m_hosts;
    };
}

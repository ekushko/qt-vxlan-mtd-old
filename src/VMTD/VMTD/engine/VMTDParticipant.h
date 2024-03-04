#pragma once

#include "../device/node/VMTDNodeDevice.h"

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

        // МЕТАДАННЫЕ

        int     index_1();
        void setIndex_1(int index_1);

        int     index_2();
        void setIndex_2(int index_2);

        // ДАННЫЕ

        VMTDNodeDevice *nodeDevice() const;

        EnRole  role() const;
        void setRole(EnRole role);

        // ОСНОВНОЙ ИНТЕРФЕЙС

        QString name_1() const;

        int     groupIndex_1();
        void setGroupIndex_1(int groupIndex_1);

        QString ip_1() const;
        void setIp_1(const QString &ip_1);

        QString mask_1() const;
        void setMask_1(const QString &mask_1);

        int     vlanId_1() const;
        void setVlanId_1(int vlanId_1);

        // ДОПОЛНИТЕЛЬНЫЙ ИНТЕРФЕЙС

        QString name_2() const;

        int     groupIndex_2();
        void setGroupIndex_2(int groupIndex_2);

        QString ip_2() const;
        void setIp_2(const QString &ip_2);

        QString mask_2() const;
        void setMask_2(const QString &mask_2);

        int     vlanId_2() const;
        void setVlanId_2(int vlanId_2);

        // МАРШРУТИЗАЦИЯ

        QString gateway() const;
        void setGateway(const QString &gateway);

        const QStringList &routes() const;
        void            setRoutes(const QStringList &routes);
        void          clearRoutes();
        void            addRoute(const QString &route);

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

        int m_index_1;
        int m_index_2;

        EnRole m_role;

        int m_groupIndex_1;
        QString m_ip_1;
        QString m_mask_1;
        int m_vlanId_1;

        int m_groupIndex_2;
        QString m_ip_2;
        QString m_mask_2;
        int m_vlanId_2;

        QString m_gateway;
        QStringList m_routes;
        QStringList m_hosts;
    };
}

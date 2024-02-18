#pragma once

#include "../VMTDDevice.h"

namespace VMTDLib
{
    class VMTDNodeDeviceForm;

    class VMTD_SHARED VMTDNodeDevice : public VMTDDevice
    {
        Q_OBJECT

    public:

        VMTDNodeDevice(QObject *parent, VMTDSettings *settings, int id);
        ~VMTDNodeDevice();

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

        QJsonObject toJson() const override;
        void      fromJson(const QJsonObject &jsonObj) override;

        // ДАННЫЕ

        QString name() const override;

        QString ip() const;
        void setIp(const QString &ip);

        EnRole  role() const;
        void setRole(EnRole role);

        QString domainName() const;
        void setDomainName(const QString &domainName);

    signals:

        void appendRequestsSignal(const QList<QPair<QString, QJsonObject>> &requests);

    public slots:

        void showFormSlot() override;

    private:

        QPointer<VMTDNodeDeviceForm> m_form;

        QString m_ip;
        EnRole m_role;
        QString m_domainName;
    };
}

#pragma once

#include "../../VMTDSettings.h"
#include "../interface/VMTDInterfaces.h"

namespace VMTDLib
{
    class VMTDNodeDeviceForm;

    class VMTD_SHARED VMTDNodeDevice : public QObject
    {
        Q_OBJECT

    public:

        VMTDNodeDevice(QObject *parent, VMTDSettings *settings, int id);
        ~VMTDNodeDevice();

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

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        // МЕТАДАННЫЕ

        bool  isOnline() const;
        void setOnline(bool isOnline);

        // ДАННЫЕ

        int id() const;

        QString name() const;

        QString ip() const;
        void setIp(const QString &ip);

        EnRole  role() const;
        void setRole(EnRole role);

        VMTDInterfaces *interfaces() const;

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDNodeDeviceForm> m_form;

        VMTDSettings *m_settings;
        VMTDInterfaces *m_interfaces;

        bool m_isOnline = false;

        int m_id;
        QString m_ip = "127.0.0.1";
        EnRole m_role = EnRole::ENDPOINT;
    };
}

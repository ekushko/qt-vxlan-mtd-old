#pragma once

#include "../VMTDSettings.h"

#include <QJsonObject>

namespace VMTDLib
{
    class VMTD_SHARED VMTDNode : public QObject
    {
        Q_OBJECT

    public:

        VMTDNode(QObject *parent, VMTDSettings *settings);

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

        bool isConnected() const;

        // ДАННЫЕ

        int     id() const;
        void setId(int id);

        QString ip() const;
        void setIp(const QString &ip);

        EnRole  role() const;
        void setRole(EnRole role);

        int     portNumber() const;
        void setPortNumber(int portNumber);

        int     currentSwitch() const;
        void setCurrentSwitch(int currentSwitch);

    private:

        VMTDSettings *m_settings;

        bool m_isOnline = false;

        int m_id = -1;
        QString m_ip = "127.0.0.1";
        EnRole m_role = EnRole::ENDPOINT;
        int m_portNumber = 0;
        int m_currentSwitch = -1;
    };
}

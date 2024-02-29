#pragma once

#include "../VMTDSettings.h"

#include <QObject>

namespace VMTDLib
{
    class VMTD_SHARED VMTDInterface : public QObject
    {
        Q_OBJECT

    public:

        VMTDInterface(QObject *parent, VMTDSettings *settings, int id);
        ~VMTDInterface();

        VMTDSettings *settings() const;

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        // ДАННЫЕ

        int     id() const;
        void setId(int id);

        QString name() const;
        void setName(const QString &name);

        QString ip() const;
        void setIp(const QString &ip);

    private:

        VMTDSettings *m_settings;

        int m_id;
        QString m_name;
        QString m_ip;
    };
}

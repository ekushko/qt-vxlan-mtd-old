#pragma once

#include "../interface/VMTDInterfaceManager.h"

namespace VMTDLib
{
    class VMTDDevice : public QObject
    {
        Q_OBJECT

    public:

        enum class EnType;

        VMTDDevice(QObject *parent, VMTDSettings *settings, EnType type, int id);

        VMTDSettings *settings() const;

        // КОНСТАНТЫ

        enum class EnType
        {
            NX_API,
            NODE
        };
        Q_ENUM(EnType)

        // ЛОГИКА

        virtual QJsonObject toJson() const;
        virtual void      fromJson(const QJsonObject &jsonObj);

        // МЕТАДАННЫЕ

        bool  isOnline() const;
        void setOnline(bool isOnline);

        // ДАННЫЕ

        EnType type() const;

        int id() const;

        virtual QString name() const = 0;

        VMTDInterfaceManager *interfaceManager() const;

    public slots:

        virtual void showFormSlot() = 0;

    protected:

        VMTDSettings *m_settings;
        VMTDInterfaceManager *m_interfaceManager;

        bool m_isOnline = false;

        EnType m_type;
        int m_id;
    };
}

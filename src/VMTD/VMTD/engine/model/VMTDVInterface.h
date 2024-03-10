#pragma once

#include "VMTDVRoute.h"

namespace VMTDLib
{
    class VMTDVInterface : public QObject
    {
        Q_OBJECT

    public:

        VMTDVInterface(QObject *parent, VMTDSettings *settings,
                       const QString &interfaceName);
        ~VMTDVInterface();

        // ЛОГИКА

        QString toString() const;

        // МЕТАДАННЫЕ

        int     index();
        void setIndex(int index);

        int     groupIndex();
        void setGroupIndex(int groupIndex);

        QString name() const;

        // ДАННЫЕ

        QString interfaceName() const;

        QString ip() const;
        void setIp(const QString &ip);

        uchar   mask() const;
        void setMask(uchar mask);

        int     vlanId() const;
        void setVlanId(int vlanId);

        const QList<VMTDVRoute> &routes() const;
        void                clearRoutes();
        void                  addRoute(const QString &network, uchar mask,
                                       const QString &gateway, int metric);

    private:

        VMTDSettings *m_settings;

        QString m_interfaceName;

        int m_index;
        int m_groupIndex;
        QString m_ip;
        uchar m_mask;
        int m_vlanId;

        QList<VMTDVRoute> m_routes;
    };
}

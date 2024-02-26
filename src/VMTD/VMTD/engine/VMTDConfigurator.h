#pragma once

#include "../VMTDSettings.h"

#include <QObject>

namespace VMTDLib
{
    class VMTDConfiguratorForm;

    class VMTDConfigurator : public QObject
    {
        Q_OBJECT

    public:

        VMTDConfigurator(QObject *parent, VMTDSettings *settings);
        ~VMTDConfigurator();

        VMTDSettings *settings() const;

        const QString &interface1() const;
        const QString &interface2() const;
        const QString &routes() const;
        const QString &hosts() const;

    public slots:

        void handleMethodSlot(const QString &method, const QJsonObject &params, bool &result);

        void showFormSlot(QWidget *parent);

    private:

        bool handleSetupInterface1(const QJsonObject &params);
        bool handleSetupInterface2(const QJsonObject &params);
        bool handleSetupInterface1Vlan(const QJsonObject &params);
        bool handleSetupInterface2Vlan(const QJsonObject &params);
        bool handleSetupRoutes(const QJsonObject &params);
        bool handleSetupHosts(const QJsonObject &params);

        bool handleClearInterface(const QJsonObject &params);
        bool handleClearRoutes(const QJsonObject &params);
        bool handleClearHosts(const QJsonObject &params);

        QPointer<VMTDConfiguratorForm> m_form;

        VMTDSettings *m_settings;

        QString m_interface_1;
        QString m_interface_2;
        QString m_routes;
        QString m_hosts;
    };
}

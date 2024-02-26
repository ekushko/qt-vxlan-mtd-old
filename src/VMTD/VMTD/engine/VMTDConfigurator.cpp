#include "VMTDConfigurator.h"
#include "VMTDConfiguratorForm.h"

#include "../VMTDRepo.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

namespace VMTDLib
{
    VMTDConfigurator::VMTDConfigurator(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDConfigurator) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDConfigurator) + " | Constructor finished");
    }

    VMTDConfigurator::~VMTDConfigurator()
    {
        m_settings->creationOut(VN_S(VMTDConfigurator) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDConfigurator) + " | Destructor finished");
    }

    VMTDSettings *VMTDConfigurator::settings() const
    {
        return m_settings;
    }

    const QString &VMTDConfigurator::interface1() const
    {
        return m_interface_1;
    }
    const QString &VMTDConfigurator::interface2() const
    {
        return m_interface_2;
    }
    const QString &VMTDConfigurator::routes() const
    {
        return m_routes;
    }
    const QString &VMTDConfigurator::hosts() const
    {
        return m_hosts;
    }

    void VMTDConfigurator::handleMethodSlot(const QString &method,
                                            const QJsonObject &params,
                                            bool &result)
    {
        if (method == MTH_SETUP_INTERFACE_1)
        {
            result = handleSetupInterface1(params);
        }
        else if (method == MTH_SETUP_ROUTES)
        {
            result = handleSetupRoutes(params);
        }
        else if (method == MTH_SETUP_HOSTS)
        {
            result = handleSetupHosts(params);
        }
        else if (method == MTH_CLEAR_INTERFACE_1)
        {
            result = handleClearInterface(params);
        }
        else if (method == MTH_CLEAR_ROUTES)
        {
            result = handleClearRoutes(params);
        }
        else if (method == MTH_CLEAR_HOSTS)
        {
            result = handleClearHosts(params);
        }
        else
        {
            result = false;
        }
    }

    void VMTDConfigurator::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDConfiguratorForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    bool VMTDConfigurator::handleSetupInterface1(const QJsonObject &params)
    {
        m_interface_1 = QString("auto %1"
                                "iface %1 inet static\n"
                                "address %2\n"
                                "netmask %3\n"
                                "gateway %4\n")
                        .arg(params[PRM_INTERFACE].toString())
                        .arg(params[PRM_IP].toString())
                        .arg(params[PRM_MASK].toString())
                        .arg(params[PRM_GATEWAY].toString());
        return true;
    }
    bool VMTDConfigurator::handleSetupInterface2(const QJsonObject &params)
    {
        m_interface_2 = QString("auto %1"
                                "iface %1 inet static\n"
                                "address %2\n"
                                "netmask %3\n"
                                "gateway %4\n")
                        .arg(params[PRM_INTERFACE].toString())
                        .arg(params[PRM_IP].toString())
                        .arg(params[PRM_MASK].toString())
                        .arg(params[PRM_GATEWAY].toString());
        return true;
    }
    bool VMTDConfigurator::handleSetupInterface1Vlan(const QJsonObject &params)
    {
        m_interface_1 = QString("auto %1.%2"
                                "iface %1 inet static\n"
                                "address %3\n"
                                "netmask %4\n"
                                "vlan-raw-device %1\n")
                        .arg(params[PRM_INTERFACE].toString())
                        .arg(params[PRM_VLAN_ID].toInt())
                        .arg(params[PRM_IP].toString())
                        .arg(params[PRM_MASK].toString());
        return true;
    }
    bool VMTDConfigurator::handleSetupInterface2Vlan(const QJsonObject &params)
    {
        m_interface_2 = QString("auto %1.%2"
                                "iface %1 inet static\n"
                                "address %3\n"
                                "netmask %4\n"
                                "vlan-raw-device %1\n")
                        .arg(params[PRM_INTERFACE].toString())
                        .arg(params[PRM_VLAN_ID].toInt())
                        .arg(params[PRM_IP].toString())
                        .arg(params[PRM_MASK].toString());
        return true;
    }
    bool VMTDConfigurator::handleSetupRoutes(const QJsonObject &params)
    {
        m_routes.clear();

        for (auto jsonVal : params[PRM_ROUTES].toArray())
        {
            const auto jsonObj = jsonVal.toObject();

            m_routes.append(QString("%1 %2 via %3\n")
                            .arg(PRM_NETWORK)
                            .arg(PRM_MASK)
                            .arg(PRM_GATEWAY));
        }

        return true;
    }
    bool VMTDConfigurator::handleSetupHosts(const QJsonObject &params)
    {
        m_hosts.clear();

        for (auto jsonVal : params[PRM_HOSTS].toArray())
        {
            const auto jsonObj = jsonVal.toObject();

            m_hosts.append(QString("%1 %2\n")
                           .arg(PRM_IP)
                           .arg(PRM_DOMAIN_NAME));
        }

        return true;
    }

    bool VMTDConfigurator::handleClearInterface(const QJsonObject &params)
    {
        if (params[PRM_INTERFACE] == "vmtd1")
        {
            m_interface_1.clear();
        }
        else if (params[PRM_INTERFACE] == "vmtd2")
        {
            m_interface_2.clear();
        }

        return true;
    }
    bool VMTDConfigurator::handleClearRoutes(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_routes.clear();

        return true;
    }
    bool VMTDConfigurator::handleClearHosts(const QJsonObject &params)
    {
        Q_UNUSED(params)

        m_hosts.clear();

        return true;
    }
}

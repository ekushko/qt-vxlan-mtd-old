#include "VMTDConnectionManager.h"
#include "VMTDConnectionManagerForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDConnectionManager::VMTDConnectionManager(QObject *parent, VMTDDeviceManager *deviceManager)
        : QObject(parent)
        , m_deviceManager(deviceManager)
        , m_settings(deviceManager->settings())
    {
        m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Constructor called");

        if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            loadSlot();
        }

        m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Constructor finished");
    }

    VMTDConnectionManager::~VMTDConnectionManager()
    {
        m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Destructor called");

        // do nothing

        m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Destructor finished");
    }

    VMTDDeviceManager *VMTDConnectionManager::deviceManager() const
    {
        return m_deviceManager;
    }

    VMTDSettings *VMTDConnectionManager::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDConnectionManager::toJson() const
    {
        QJsonObject jsonObj;

        QJsonArray jsonArr;

        for (auto it = m_connections.begin(); it != m_connections.end(); ++it)
        {
            QJsonObject connObj;

            connObj["deviceId_1"] = it.key().first;
            connObj["interfaceId_1"] = it.key().second;

            connObj["deviceId_2"] = it.value().first;
            connObj["interfaceId_2"] = it.value().second;

            jsonArr.append(connObj);
        }

        jsonObj[VN_ME(m_connections)] = jsonArr;

        return jsonObj;
    }
    void VMTDConnectionManager::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        const auto jsonArr = jsonObj[VN_ME(m_connections)].toArray();

        for (int i = 0; i < jsonArr.size(); ++i)
        {
            const auto connObj = jsonArr.at(i).toObject();

            auto pair_1 = qMakePair(connObj["deviceId_1"].toInt(), connObj["interfaceId_1"].toInt());
            auto pair_2 = qMakePair(connObj["deviceId_2"].toInt(), connObj["interfaceId_2"].toInt());
            m_connections[pair_1] = pair_2;
            m_connections[pair_2] = pair_1;
        }
    }

    void VMTDConnectionManager::createConnection(VMTDDevice *device_1, VMTDInterface *interface_1,
                                                 VMTDDevice *device_2, VMTDInterface *interface_2)
    {
        auto isValid = device_1 != nullptr && interface_1 != nullptr
                       && device_2 != nullptr && interface_2 != nullptr;

        if (!isValid)
        {
            m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Nullptr is in args!");
            return;
        }

        isValid = device_1 != device_2;

        if (!isValid)
        {
            m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Loop connection is deprecated!");
            return;
        }

        isValid &= device_1->interfaceManager()->contains(interface_1)
                   && device_2->interfaceManager()->contains(interface_2);

        if (!isValid)
        {
            m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Invalid connection!");
            return;
        }

        isValid &= !(device_1->type() == VMTDDevice::EnType::NODE
                     && device_2->type() == VMTDDevice::EnType::NODE);

        if (!isValid)
        {
            m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Connection node to node is deprecated!");
            return;
        }

        auto pair_1 = qMakePair(device_1->id(), interface_1->id());
        auto pair_2 = qMakePair(device_2->id(), interface_2->id());
        auto pair_1_old = m_connections[pair_1];
        auto pair_2_old = m_connections[pair_2];

        m_connections.remove(pair_1_old);
        m_connections.remove(pair_2_old);
        m_connections[pair_1] = pair_2;
        m_connections[pair_2] = pair_1;
    }

    void VMTDConnectionManager::removeConnection(VMTDDevice *device_1, VMTDInterface *interface_1,
                                                 VMTDDevice *device_2, VMTDInterface *interface_2)
    {
        auto isValid = device_1 != nullptr && interface_1 != nullptr
                       && device_2 != nullptr && interface_2 != nullptr;

        if (!isValid)
        {
            m_settings->debugOut(VN_S(VMTDConnectionManager) + " | Nullptr is in args!");
            return;
        }

        auto pair_1 = qMakePair(device_1->id(), interface_1->id());
        auto pair_2 = qMakePair(device_2->id(), interface_2->id());
        m_connections.remove(pair_1);
        m_connections.remove(pair_2);
    }

    QPair<VMTDDevice *, VMTDInterface *> VMTDConnectionManager::connectedDeviceInterface(
        VMTDDevice *device, VMTDInterface *interface)
    {
        const auto pair = qMakePair(device->id(), interface->id());

        if (!m_connections.contains(pair))
        {
            m_settings->debugOut(QString("%1 | Connection for device %2 interface id: %3 [%4] not found!")
                                 .arg(VN_S(VMTDConnectionManager))
                                 .arg(device->name())
                                 .arg(interface->id())
                                 .arg(interface->name()));
            return QPair<VMTDDevice *, VMTDInterface *>();
        }

        auto _connectedDevice = m_deviceManager->device(m_connections[pair].first);

        if (_connectedDevice == nullptr)
        {
            m_settings->debugOut(QString("%1 | Connected device for device %2 interface id: %3 [%4] not found!")
                                 .arg(VN_S(VMTDConnectionManager))
                                 .arg(device->name())
                                 .arg(interface->id())
                                 .arg(interface->name()));
        }
        else
        {
            m_settings->debugOut(QString("%1 | Connected device %2 for device %3 interface id: %4 [%5]")
                                 .arg(VN_S(VMTDConnectionManager))
                                 .arg(_connectedDevice->name())
                                 .arg(device->name())
                                 .arg(interface->id())
                                 .arg(interface->name()));
        }

        auto _connectedInterface = _connectedDevice->interfaceManager()->interface(
                                           m_connections[pair].second);

        if (_connectedInterface == nullptr)
        {
            m_settings->debugOut(
                QString("%1 | Connected interface for device %2 interface id: %3 [%4] not found!")
                .arg(VN_S(VMTDConnectionManager))
                .arg(device->name())
                .arg(interface->id())
                .arg(interface->name()));
        }
        else
        {
            m_settings->debugOut(
                QString("%1 | Connected interface id: %2 [%3] for device %4 interface id: %5 [%6]")
                .arg(VN_S(VMTDConnectionManager))
                .arg(_connectedInterface->id())
                .arg(_connectedInterface->name())
                .arg(device->name())
                .arg(interface->id())
                .arg(interface->name()));
        }

        return qMakePair(_connectedDevice, _connectedInterface);
    }

    void VMTDConnectionManager::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDConnectionManagerForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDConnectionManager::saveSlot()
    {
        m_settings->setConnectionManagerObj(toJson());
        m_settings->save();
    }
    void VMTDConnectionManager::loadSlot()
    {
        fromJson(m_settings->connectionManagerObj());
    }
}

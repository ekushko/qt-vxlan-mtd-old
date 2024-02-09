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
        auto isValid = device_1 != device_2;

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
        m_connections[pair_1] = pair_2;
        m_connections[pair_2] = pair_1;
    }

    void VMTDConnectionManager::removeConnection(VMTDDevice *device_1, VMTDInterface *interface_1,
                                                 VMTDDevice *device_2, VMTDInterface *interface_2)
    {
        auto pair_1 = qMakePair(device_1->id(), interface_1->id());
        auto pair_2 = qMakePair(device_2->id(), interface_2->id());
        m_connections.remove(pair_1);
        m_connections.remove(pair_2);
    }

    VMTDDevice *VMTDConnectionManager::connectedDevice(VMTDDevice *device,
                                                       VMTDInterface *interface)
    {
        const auto pair = qMakePair(device->id(), interface->id());

        if (!m_connections.contains(pair))
            return nullptr;

        return m_deviceManager->nxApiDevice(m_connections[pair].first);
    }
    VMTDInterface *VMTDConnectionManager::connectedInterface(VMTDDevice *device,
                                                             VMTDInterface *interface)
    {
        auto _connectedDevice = connectedDevice(device, interface);

        if (_connectedDevice == nullptr)
            return nullptr;

        const auto pair = qMakePair(device->id(), interface->id());

        return _connectedDevice->interfaceManager()->interface(m_connections[pair].second);
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

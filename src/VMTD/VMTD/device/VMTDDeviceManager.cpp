#include "VMTDDeviceManager.h"
#include "VMTDDeviceManagerForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDDeviceManager::VMTDDeviceManager(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->debugOut(VN_S(VMTDDeviceManager) + " | Constructor called");

        if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            loadSlot();
        }

        m_settings->debugOut(VN_S(VMTDDeviceManager) + " | Constructor finished");
    }

    VMTDDeviceManager::~VMTDDeviceManager()
    {
        m_settings->debugOut(VN_S(VMTDDeviceManager) + " | Destructor called");

        // do nothing

        m_settings->debugOut(VN_S(VMTDDeviceManager) + " | Destructor finished");
    }

    VMTDSettings *VMTDDeviceManager::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDDeviceManager::toJson() const
    {
        QJsonObject jsonObj;

        QJsonArray nodeDevicesArr, nxApiDevicesArr;

        for (auto nodeDevice : m_nodeDevices.values())
            nodeDevicesArr.append(nodeDevice->toJson());

        jsonObj[VN_ME(m_nodeDevices)] = nodeDevicesArr;

        for (auto nxApiDevice : m_nxApiDevices.values())
            nxApiDevicesArr.append(nxApiDevice->toJson());

        jsonObj[VN_ME(m_nxApiDevices)] = nxApiDevicesArr;

        return jsonObj;
    }
    void VMTDDeviceManager::fromJson(const QJsonObject &jsonObj)
    {
        qDeleteAll(m_nodeDevices.values());
        m_nodeDevices.clear();

        qDeleteAll(m_nxApiDevices.values());
        m_nxApiDevices.clear();

        if (jsonObj.isEmpty())
            return;

        auto nodeDevicesArr = jsonObj[VN_ME(m_nodeDevices)].toArray();

        for (int i = 0; i < nodeDevicesArr.size(); ++i)
        {
            auto nodeDevice = new VMTDNodeDevice(this, m_settings, -1);
            nodeDevice->fromJson(nodeDevicesArr[i].toObject());
            m_nodeDevices[nodeDevice->id()] = nodeDevice;
            m_devices[nodeDevice->id()] = nodeDevice;
        }

        auto nxApiDevicesArr = jsonObj[VN_ME(m_nxApiDevices)].toArray();

        for (int i = 0; i < nxApiDevicesArr.size(); ++i)
        {
            auto nxApiDevice = new VMTDNxApiDevice(this, m_settings, -1);
            nxApiDevice->fromJson(nxApiDevicesArr[i].toObject());
            m_nxApiDevices[nxApiDevice->id()] = nxApiDevice;
            m_devices[nxApiDevice->id()] = nxApiDevice;
        }
    }

    const QMap<int, VMTDDevice *> &VMTDDeviceManager::devices() const
    {
        return m_devices;
    }
    VMTDDevice *VMTDDeviceManager::device(int id) const
    {
        return m_devices.value(id, nullptr);
    }

    const QMap<int, VMTDNodeDevice *> &VMTDDeviceManager::nodeDevices() const
    {
        return m_nodeDevices;
    }
    VMTDNodeDevice *VMTDDeviceManager::nodeDevice(int id) const
    {
        return m_nodeDevices.value(id, nullptr);
    }
    VMTDNodeDevice *VMTDDeviceManager::nodeDevice(const QString &ip) const
    {
        // *INDENT-OFF*
        auto res = std::find_if(m_nodeDevices.begin(), m_nodeDevices.end(),
                                [ip](VMTDNodeDevice *nodeDevice)
        {
            return nodeDevice->ip() == ip;
        });
        // *INDENT-ON*

        if (res == m_nodeDevices.end())
            return nullptr;

        return *res;
    }
    bool VMTDDeviceManager::addNodeDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nodeDevices.contains(id))
        {
            auto nodeDevice = new VMTDNodeDevice(this, m_settings, id);;
            m_nodeDevices[id] = nodeDevice;
            m_devices[id] = nodeDevice;
            return true;
        }

        return false;
    }
    bool VMTDDeviceManager::removeNodeDevice(int id)
    {
        if (!m_nodeDevices.contains(id))
            return false;

        delete m_nodeDevices[id];
        m_nodeDevices.remove(id);
        m_devices.remove(id);
        return true;
    }

    const QMap<int, VMTDNxApiDevice *> &VMTDDeviceManager::nxApiDevices() const
    {
        return m_nxApiDevices;
    }
    VMTDNxApiDevice *VMTDDeviceManager::nxApiDevice(int id) const
    {
        return m_nxApiDevices.value(id, nullptr);
    }
    VMTDNxApiDevice *VMTDDeviceManager::nxApiDevice(const QUrl &url) const
    {
        // *INDENT-OFF*
        auto res = std::find_if(m_nxApiDevices.begin(), m_nxApiDevices.end(),
                                [url](VMTDNxApiDevice *nxApiDevice)
        {
            return nxApiDevice->url().toString(QUrl::RemoveUserInfo)
                   == url.toString(QUrl::RemoveUserInfo);
        });
        // *INDENT-ON*

        if (res == m_nxApiDevices.end())
            return nullptr;

        return *res;
    }
    bool VMTDDeviceManager::addNxApiDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nxApiDevices.contains(id))
        {
            auto nxApiDevice = new VMTDNxApiDevice(this, m_settings, id);
            m_nxApiDevices[id] = nxApiDevice;
            m_devices[id] = nxApiDevice;
            return true;
        }

        return false;
    }
    bool VMTDDeviceManager::removeNxApiDevice(int id)
    {
        if (!m_nxApiDevices.contains(id))
            return false;

        delete m_nxApiDevices[id];
        m_nxApiDevices.remove(id);
        m_devices.remove(id);
        return true;
    }

    void VMTDDeviceManager::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDDeviceManagerForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDDeviceManager::saveSlot()
    {
        m_settings->setDeviceManagerObj(toJson());
        m_settings->save();
    }
    void VMTDDeviceManager::loadSlot()
    {
        fromJson(m_settings->deviceManagerObj());
    }
}

#include "VMTDNet.h"
#include "VMTDNetForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDNet::VMTDNet(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            loadSlot();
        }
        else if (m_settings->nodeType() == VMTDNodeType::CLIENT)
        {
            const auto id = 1;

            auto nodeDevice = new VMTDNodeDevice(this, m_settings, id);
            nodeDevice->setIp(m_settings->serverIp());
            m_nodeDevices[id] = nodeDevice;
        }
    }

    VMTDSettings *VMTDNet::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDNet::toJson() const
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
    void VMTDNet::fromJson(const QJsonObject &jsonObj)
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
        }

        auto nxApiDevicesArr = jsonObj[VN_ME(m_nxApiDevices)].toArray();

        for (int i = 0; i < nxApiDevicesArr.size(); ++i)
        {
            auto nxApiDevice = new VMTDNxApiDevice(this, m_settings, -1);
            nxApiDevice->fromJson(nxApiDevicesArr[i].toObject());
            m_nxApiDevices[nxApiDevice->id()] = nxApiDevice;
        }
    }

    const QMap<int, VMTDNodeDevice *> &VMTDNet::nodeDevices() const
    {
        return m_nodeDevices;
    }
    VMTDNodeDevice *VMTDNet::nodeDevice(int id) const
    {
        return m_nodeDevices.value(id, nullptr);
    }
    VMTDNodeDevice *VMTDNet::nodeDevice(const QString &ip) const
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
    bool VMTDNet::addNodeDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nodeDevices.contains(id))
        {
            m_nodeDevices[id] = new VMTDNodeDevice(this, m_settings, id);
            return true;
        }

        return false;
    }
    bool VMTDNet::removeNodeDevice(int id)
    {
        if (!m_nodeDevices.contains(id))
            return false;

        delete m_nodeDevices[id];
        m_nodeDevices.remove(id);
        return true;
    }

    const QMap<int, VMTDNxApiDevice *> &VMTDNet::nxApiDevices() const
    {
        return m_nxApiDevices;
    }
    VMTDNxApiDevice *VMTDNet::nxApiDevice(int id) const
    {
        return m_nxApiDevices.value(id, nullptr);
    }
    VMTDNxApiDevice *VMTDNet::nxApiDevice(const QUrl &url) const
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
    bool VMTDNet::addNxApiDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nxApiDevices.contains(id))
        {
            m_nxApiDevices[id] = new VMTDNxApiDevice(this, m_settings, id);
            return true;
        }

        return false;
    }
    bool VMTDNet::removeNxApiDevice(int id)
    {
        if (!m_nxApiDevices.contains(id))
            return false;

        delete m_nxApiDevices[id];
        m_nxApiDevices.remove(id);
        return true;
    }

    void VMTDNet::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDNetForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNet::saveSlot()
    {
        m_settings->setNetObj(toJson());
        m_settings->save();
    }
    void VMTDNet::loadSlot()
    {
        fromJson(m_settings->netObj());
    }
}

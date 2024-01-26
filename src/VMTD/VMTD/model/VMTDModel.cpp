#include "VMTDModel.h"
#include "VMTDModelForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDModel::VMTDModel(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        loadSlot();
    }

    VMTDSettings *VMTDModel::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDModel::toJson() const
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
    void VMTDModel::fromJson(const QJsonObject &jsonObj)
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
            auto nodeDevice = new VMTDNodeDevice(this, m_settings);
            nodeDevice->fromJson(nodeDevicesArr[i].toObject());
            m_nodeDevices[nodeDevice->id()] = nodeDevice;
        }

        auto nxApiDevicesArr = jsonObj[VN_ME(m_nxApiDevices)].toArray();

        for (int i = 0; i < nxApiDevicesArr.size(); ++i)
        {
            auto nxApiDevice = new VMTDNxApiDevice(this, m_settings);
            nxApiDevice->fromJson(nxApiDevicesArr[i].toObject());
            m_nxApiDevices[nxApiDevice->id()] = nxApiDevice;
        }
    }

    const QMap<int, VMTDNodeDevice *> &VMTDModel::nodeDevices() const
    {
        return m_nodeDevices;
    }
    VMTDNodeDevice *VMTDModel::nodeDevice(int id) const
    {
        return m_nodeDevices.value(id, nullptr);
    }
    VMTDNodeDevice *VMTDModel::nodeDevice(const QString &ip) const
    {
        const auto nodeDevices_ = m_nodeDevices.values();

        // *INDENT-OFF*
        const auto res = std::find_if(nodeDevices_.begin(), nodeDevices_.end(),
                                      [ip](VMTDNodeDevice *nodeDevice)
        {
            return nodeDevice->ip() == ip;
        });
        // *INDENT-ON*

        if (res == nodeDevices_.end())
            return nullptr;

        return *res;
    }
    bool VMTDModel::addNodeDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nodeDevices.contains(id))
        {
            m_nodeDevices[id] = new VMTDNodeDevice(this, m_settings);
            return true;
        }

        return false;
    }
    bool VMTDModel::removeNodeDevice(int id)
    {
        if (!m_nodeDevices.contains(id))
            return false;

        delete m_nodeDevices[id];
        m_nodeDevices.remove(id);
        return true;
    }

    const QMap<int, VMTDNxApiDevice *> &VMTDModel::nxApiDevices() const
    {
        return m_nxApiDevices;
    }
    VMTDNxApiDevice *VMTDModel::nxApiDevice(int id) const
    {
        return m_nxApiDevices.value(id, nullptr);
    }
    VMTDNxApiDevice *VMTDModel::nxApiDevice(const QUrl &url) const
    {
        const auto nxApiDevices_ = m_nxApiDevices.values();

        // *INDENT-OFF*
        const auto res = std::find_if(nxApiDevices_.begin(), nxApiDevices_.end(),
                                      [url](VMTDNxApiDevice *nodeDevice)
        {
            return nodeDevice->url().toString(QUrl::RemoveUserInfo)
                    == url.toString(QUrl::RemoveUserInfo);
        });
        // *INDENT-ON*

        if (res == nxApiDevices_.end())
            return nullptr;

        return *res;
    }
    bool VMTDModel::addNxApiDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nxApiDevices.contains(id))
        {
            m_nxApiDevices[id] = new VMTDNxApiDevice(this, m_settings);
            return true;
        }

        return false;
    }
    bool VMTDModel::removeNxApiDevice(int id)
    {
        if (!m_nxApiDevices.contains(id))
            return false;

        delete m_nxApiDevices[id];
        m_nxApiDevices.remove(id);
        return true;
    }

    void VMTDModel::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDModelForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDModel::saveSlot()
    {
        m_settings->setModelObj(toJson());
        m_settings->save();
    }
    void VMTDModel::loadSlot()
    {
        fromJson(m_settings->modelObj());
    }
}

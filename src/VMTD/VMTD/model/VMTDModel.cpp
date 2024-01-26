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

    const QMap<int, VMTDNodeDevice *> &VMTDModel::nodeDevices() const
    {
        return m_nodeDevices;
    }
    VMTDNodeDevice *VMTDModel::nodeDevice(int id) const
    {
        return m_nodeDevices.value(id, nullptr);
    }
    bool VMTDModel::addNodeDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nodeDevices.contains(id))
        {
            m_nodeDevices[id] = new VMTDNodeDevice(this, m_settings, id);
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
    bool VMTDModel::addNxApiDevice()
    {
        const auto id = m_settings->generateId();

        if (!m_nxApiDevices.contains(id))
        {
            m_nxApiDevices[id] = new VMTDNxApiDevice(this, m_settings, id);
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

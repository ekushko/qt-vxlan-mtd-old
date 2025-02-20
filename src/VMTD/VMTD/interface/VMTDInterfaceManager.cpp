#include "VMTDInterfaceManager.h"
#include "VMTDInterfaceManagerForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDInterfaceManager::VMTDInterfaceManager(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDInterfaceManager) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDInterfaceManager) + " | Constructor finished");
    }

    VMTDInterfaceManager::~VMTDInterfaceManager()
    {
        m_settings->creationOut(VN_S(VMTDInterfaceManager) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDInterfaceManager) + " | Destructor finished");
    }

    VMTDSettings *VMTDInterfaceManager::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDInterfaceManager::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_onlyOneMode)] = m_onlyOneMode;

        jsonObj[VN_ME(m_idCounter)] = m_idCounter;

        QJsonArray jsonArr;

        for (auto interface : m_interfaces)
            jsonArr.append(interface->toJson());

        jsonObj[VN_ME(m_interfaces)] = jsonArr;

        return jsonObj;
    }
    void VMTDInterfaceManager::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_onlyOneMode = jsonObj[VN_ME(m_onlyOneMode)].toBool(m_onlyOneMode);

        m_idCounter = jsonObj[VN_ME(m_idCounter)].toInt(m_idCounter);

        const auto jsonArr = jsonObj[VN_ME(m_interfaces)].toArray();

        qDeleteAll(m_interfaces.values());
        m_interfaces.clear();

        for (int i = 0; i < jsonArr.size(); ++i)
        {
            auto interface = new VMTDInterface(this, m_settings, -1);
            interface->fromJson(jsonArr.at(i).toObject());

            if (interface->id() == -1)
                interface->setId(++m_idCounter);

            if (!m_interfaces.contains(interface->id()))
                m_interfaces[interface->id()] = interface;

            if (m_onlyOneMode)
                break;
        }
    }

    bool VMTDInterfaceManager::onlyOneMode() const
    {
        return m_onlyOneMode;
    }
    void VMTDInterfaceManager::setOnlyOneMode(bool onlyOneMode)
    {
        m_onlyOneMode = onlyOneMode;
    }

    bool VMTDInterfaceManager::contains(VMTDInterface *interface) const
    {
        return m_interfaces.values().contains(interface);
    }

    const QMap<int, VMTDInterface *> &VMTDInterfaceManager::interfaces() const
    {
        return m_interfaces;
    }
    VMTDInterface *VMTDInterfaceManager::interface(int id) const
    {
        return m_interfaces.value(id, nullptr);
    }
    bool VMTDInterfaceManager::addInterface()
    {
        const auto id = ++m_idCounter;

        if (!m_onlyOneMode || m_interfaces.size() == 0)
        {
            m_interfaces[id] = new VMTDInterface(this, m_settings, id);
            emit interfaceCreatedSignal(id);
            return true;
        }

        return false;
    }
    bool VMTDInterfaceManager::removeInterface(int id)
    {
        if (!m_interfaces.contains(id))
            return false;

        emit interfaceRemovedSignal(id);
        delete m_interfaces[id];
        m_interfaces.remove(id);
        return true;
    }

    void VMTDInterfaceManager::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDInterfaceManagerForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }
}

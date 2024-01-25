#include "VMTDInterfaces.h"

#include "../../VMTDRepo.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDInterfaces::VMTDInterfaces(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }

    QJsonObject VMTDInterfaces::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_onlyOneMode)] = m_onlyOneMode;

        QJsonArray jsonArr;

        for (auto interface : m_interfaces)
            jsonArr.append(interface->toJson());

        jsonObj[VN_ME(m_interfaces)] = jsonArr;

        return jsonObj;
    }
    void VMTDInterfaces::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_onlyOneMode = jsonObj[VN_ME(m_onlyOneMode)].toBool();

        const auto jsonArr = jsonObj[VN_ME(m_interfaces)].toArray();

        qDeleteAll(m_interfaces.values());
        m_interfaces.clear();
        m_idCounter = 0;

        for (int i = 0; i < jsonArr.size(); ++i)
        {
            auto interface = new VMTDInterface(this, m_settings, VMTDRepo::generateId());
            interface->fromJson(jsonArr.at(i).toObject());

            if (!m_interfaces.contains(interface->id()))
            {
                m_interfaces[interface->id()] = interface;
                m_idCounter = qMax(m_idCounter, interface->id());
            }
        }
    }

    bool VMTDInterfaces::onlyOneMode() const
    {
        return m_onlyOneMode;
    }
    void VMTDInterfaces::setOnlyOneMode(bool onlyOneMode)
    {
        m_onlyOneMode = onlyOneMode;
    }

    const QMap<int, VMTDInterface *> &VMTDInterfaces::interfaces() const
    {
        return m_interfaces;
    }
    VMTDInterface *VMTDInterfaces::interface(int id) const
    {
        return m_interfaces.value(id, nullptr);
    }
    bool VMTDInterfaces::addInterface()
    {
        ++m_idCounter;

        if (!m_onlyOneMode || m_interfaces.size() == 0)
        {
            m_interfaces[m_idCounter] = new VMTDInterface(this, m_settings, m_idCounter);
            emit interfaceCreatedSignal(m_idCounter);
            return true;
        }

        return false;
    }
    bool VMTDInterfaces::removeInterface(int id)
    {
        if (!m_interfaces.contains(id))
            return false;

        emit interfaceRemovedSignal(id);
        delete m_interfaces[id];
        m_interfaces.remove(id);
        return true;
    }
}

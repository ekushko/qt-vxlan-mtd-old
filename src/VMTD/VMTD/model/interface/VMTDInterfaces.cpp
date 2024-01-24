#include "VMTDInterfaces.h"

#include <algorithm>

namespace VMTDLib
{
    VMTDInterfaces::VMTDInterfaces(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }

    const QMap<int, VMTDInterface *> &VMTDInterfaces::interfaces() const
    {
        return m_interfaces;
    }
    VMTDInterface *VMTDInterfaces::interface(int id) const
    {
        return m_interfaces.value(id, nullptr);
    }
    void VMTDInterfaces::addInterface(VMTDInterface *interface)
    {
        if (!m_interfaces.contains(interface->id())
            && m_interfaces.values().contains(interface))
            m_interfaces[interface->id()] = interface;
        else
            delete interface;
    }
    void VMTDInterfaces::removeInterface(int id)
    {
        if (!m_interfaces.contains(id))
            return;

        delete m_interfaces[id];
        m_interfaces.remove(id);
    }
}

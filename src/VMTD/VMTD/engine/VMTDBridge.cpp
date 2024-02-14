#include "VMTDBridge.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDBridge::VMTDBridge(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDBridge) + " | Constructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDBridge) + " | Constructor finished");
    }
    VMTDBridge::~VMTDBridge()
    {
        m_settings->creationOut(VN_S(VMTDBridge) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDBridge) + " | Destructor finished");
    }

    VMTDNxApiDevice *VMTDBridge::device_1() const
    {
        return m_device_1;
    }
    VMTDInterface *VMTDBridge::interface_1() const
    {
        return m_interface_1;
    }

    VMTDNxApiDevice *VMTDBridge::device_2() const
    {
        return m_device_2;
    }
    VMTDInterface *VMTDBridge::interface_2() const
    {
        return m_interface_2;
    }

    void VMTDBridge::set(VMTDNxApiDevice *device_1, VMTDInterface *interface_1,
                         VMTDNxApiDevice *device_2, VMTDInterface *interface_2)
    {
        m_device_1 = device_1;
        m_interface_1 = interface_1;

        m_device_2 = device_2;
        m_interface_2 = interface_2;
    }
}

#pragma once

#include "../../device/nxapi/VMTDNxApiDevice.h"

namespace VMTDLib
{
    class VMTD_SHARED VMTDBridge : public QObject
    {
        Q_OBJECT

    public:

        VMTDBridge(QObject *parent, VMTDSettings *settings);
        ~VMTDBridge();

        VMTDNxApiDevice *device_1() const;
        VMTDInterface *interface_1() const;

        VMTDNxApiDevice *device_2() const;
        VMTDInterface *interface_2() const;

        void set(VMTDNxApiDevice *device_1, VMTDInterface *interface_1,
                 VMTDNxApiDevice *device_2, VMTDInterface *interface_2);

    private:

        VMTDSettings *m_settings;

        VMTDNxApiDevice *m_device_1;
        VMTDInterface *m_interface_1;

        VMTDNxApiDevice *m_device_2;
        VMTDInterface *m_interface_2;
    };
}

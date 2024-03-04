#pragma once

#include "node/VMTDNodeDevice.h"
#include "nxapi/VMTDNxApiDevice.h"

namespace VMTDLib
{
    class VMTDDeviceManagerForm;

    class VMTD_SHARED VMTDDeviceManager : public QObject
    {
        Q_OBJECT

    public:

        VMTDDeviceManager(QObject *parent, VMTDSettings *settings);
        ~VMTDDeviceManager();

        VMTDSettings *settings() const;

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        // ДАННЫЕ

        const QMap<int, VMTDDevice *> &devices() const;
        VMTDDevice *device(int id) const;

        const QMap<int, VMTDNodeDevice *> &nodeDevices() const;
        VMTDNodeDevice *nodeDevice(int id) const;
        VMTDNodeDevice *nodeDevice(const QString &ip) const;
        bool         addNodeDevice();
        bool      removeNodeDevice(int id);

        const QMap<int, VMTDNxApiDevice *> &nxApiDevices() const;
        VMTDNxApiDevice *nxApiDevice(int id) const;
        VMTDNxApiDevice *nxApiDevice(const QUrl &url) const;
        bool          addNxApiDevice();
        bool       removeNxApiDevice(int id);

    public slots:

        void showFormSlot();

        void saveSlot();
        void loadSlot();

    private:

        QPointer<VMTDDeviceManagerForm> m_form;

        VMTDSettings *m_settings;

        QMap<int, VMTDDevice *>           m_devices;
        QMap<int, VMTDNodeDevice *>   m_nodeDevices;
        QMap<int, VMTDNxApiDevice *> m_nxApiDevices;
    };
}

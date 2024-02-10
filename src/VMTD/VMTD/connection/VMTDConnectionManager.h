#pragma once

#include "../device/VMTDDeviceManager.h"

namespace VMTDLib
{
    class VMTDConnectionManagerForm;

    class VMTDConnectionManager : public QObject
    {
        Q_OBJECT

    public:

        VMTDConnectionManager(QObject *parent, VMTDDeviceManager *deviceManager);
        ~VMTDConnectionManager();

        VMTDDeviceManager *deviceManager() const;

        VMTDSettings *settings() const;

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        void createConnection(VMTDDevice *device_1, VMTDInterface *interface_1,
                              VMTDDevice *device_2, VMTDInterface *interface_2);
        void removeConnection(VMTDDevice *device_1, VMTDInterface *interface_1,
                              VMTDDevice *device_2, VMTDInterface *interface_2);

        // ДАННЫЕ

        QPair<VMTDDevice *, VMTDInterface *> connectedDeviceInterface(VMTDDevice *device,
                                                                      VMTDInterface *interface);

    public slots:

        void showFormSlot(QWidget *parent = nullptr);

        void saveSlot();
        void loadSlot();

    private:

        QPointer<VMTDConnectionManagerForm> m_form;

        VMTDDeviceManager *m_deviceManager;
        VMTDSettings *m_settings;

        QMap<QPair<int, int>, QPair<int, int>> m_connections;
    };
}

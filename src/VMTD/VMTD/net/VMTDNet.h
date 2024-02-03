#pragma once

#include "../VMTDSettings.h"

#include "node/VMTDNodeDevice.h"
#include "nxapi/VMTDNxApiDevice.h"

namespace VMTDLib
{
    class VMTDNetForm;

    class VMTD_SHARED VMTDNet : public QObject
    {
        Q_OBJECT

    public:

        VMTDNet(QObject *parent, VMTDSettings *settings);

        VMTDSettings *settings() const;

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        // ДАННЫЕ

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

    signals:

        void applySignal();

    public slots:

        void showFormSlot(QWidget *parent = nullptr);

        void saveSlot();
        void loadSlot();

    private:

        QPointer<VMTDNetForm> m_form;

        VMTDSettings *m_settings;

        QMap<int, VMTDNodeDevice *> m_nodeDevices;
        QMap<int, VMTDNxApiDevice *> m_nxApiDevices;
    };
}

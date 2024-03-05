#pragma once

#include "VMTDSettings.h"

#include "adapter/nxapi/VMTDNxApiServer.h"
#include "adapter/node/VMTDNodeServer.h"
#include "adapter/node/VMTDNodeClient.h"
#include "protocol/VMTDProtocol.h"

#include "device/VMTDDeviceManager.h"
#include "connection/VMTDConnectionManager.h"

#include "engine/VMTDEngine.h"
#include "engine/VMTDConfigurator.h"

#include <QThread>

namespace VMTDLib
{
    class VMTDMainForm;

    class VMTDControllerForm;

    class VMTDController : public QThread
    {
        Q_OBJECT

    public:

        VMTDController(QObject *parent, const QString &systemName);
        ~VMTDController();

        QWidget *form() const;

        VMTDSettings *settings() const;

        VMTDNxApiServer *nxApiServer() const;

        VMTDNodeServer *nodeServer() const;

        VMTDNodeClient *nodeClient() const;

        VMTDProtocol *protocol() const;

        VMTDDeviceManager *deviceManager() const;

        VMTDConnectionManager *connectionManager() const;

        VMTDEngine *engine() const;

        VMTDConfigurator *configurator() const;

    signals:

        void restartedSignal();

    public slots:

        void showMainFormSlot();

        void showFormSlot();

        void startController();
        void  stopController();

    protected:

        void run() override;

    private:

        QPointer<VMTDMainForm> m_mainForm;
        QPointer<VMTDControllerForm> m_form;

        VMTDSettings *m_settings;

        VMTDNodeType m_nodeType;

        VMTDNxApiServer *m_nxApiServer;

        VMTDNodeServer *m_nodeServer;

        VMTDNodeClient *m_nodeClient;

        VMTDProtocol *m_protocol;

        VMTDDeviceManager *m_deviceManager;

        VMTDConnectionManager *m_connectionManager;

        VMTDEngine *m_engine;

        VMTDConfigurator *m_configurator;

    private slots:

        void restartSlot();

        void  startedSlot();
        void finishedSlot();
    };
}

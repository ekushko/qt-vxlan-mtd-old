#pragma once

#include "VMTDSettings.h"

#include "adapter/nxapi/VMTDNxApiServer.h"
#include "adapter/node/VMTDNodeServer.h"
#include "adapter/node/VMTDNodeClient.h"
#include "protocol/VMTDProtocol.h"

#include "net/VMTDNet.h"

#include <QThread>

namespace VMTDLib
{
    class VMTDControllerForm;

    class VMTDController : public QThread
    {
        Q_OBJECT

    public:

        VMTDController(QObject *parent, VMTDSettings *settings);
        ~VMTDController();

        VMTDSettings *settings() const;

        VMTDNxApiServer *nxApiServer() const;

        VMTDNodeServer *nodeServer() const;

        VMTDNodeClient *nodeClient() const;

        VMTDProtocol *protocol() const;

        VMTDNet *net() const;

    public slots:

        void showFormSlot();

        void startController();
        void  stopController();

    protected:

        void run() override;

    private:

        QPointer<VMTDControllerForm> m_form;

        VMTDSettings *m_settings;

        VMTDNxApiServer *m_nxApiServer;

        VMTDNodeServer *m_nodeServer;

        VMTDNodeClient *m_nodeClient;

        VMTDProtocol *m_protocol;

        VMTDNet *m_net;

    private slots:

        void  startedSlot();
        void finishedSlot();
    };
}

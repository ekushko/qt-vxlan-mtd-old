#pragma once

#include "VMTDSettings.h"

#include "adapter/VMTDNxApiAdapter.h"
#include "adapter/VMTDNodeServer.h"
#include "adapter/VMTDNodeClient.h"

#include <QThread>
#include <QNetworkAccessManager>

namespace VMTDLib
{
    class VMTDControllerForm;

    class VMTDController : public QThread
    {
        Q_OBJECT

    public:

        VMTDController(QObject *parent, VMTDSettings *settings);
        ~VMTDController();

        void showForm();

        VMTDSettings *settings() const;

        const QVector<VMTDNxApiAdapter *> &nxApiAdapters() const;

        VMTDNodeServer *nodeServer() const;

        VMTDNodeClient *nodeClient() const;

    public slots:

        void startController();
        void  stopController();

    protected:

        void run() override;

    private:

        QPointer<VMTDControllerForm> m_form;

        VMTDSettings *m_settings;

        QNetworkAccessManager *m_netManager;

        QVector<VMTDNxApiAdapter *> m_nxApiAdapters;

        VMTDNodeServer *m_nodeServer;

        VMTDNodeClient *m_nodeClient;

    private slots:

        void  startedSlot();
        void finishedSlot();
    };
}

#include "VMTDController.h"
#include "VMTDControllerForm.h"

#include "VMTDRepo.h"

#include <QCoreApplication>

namespace VMTDLib
{
    VMTDController::VMTDController(QObject *parent, VMTDSettings *settings)
        : QThread{parent}
        , m_settings(settings)
    {
        m_settings->debugOut(VN_S(VMTDController) + " was created");

        connect(this, &VMTDController::started, this, &VMTDController::startedSlot);
        connect(this, &VMTDController::finished, this, &VMTDController::finishedSlot);
    }

    VMTDController::~VMTDController()
    {
        m_settings->debugOut(VN_S(VMTDController) + " was deleted");

        if (m_form != nullptr)
            delete m_form;
    }

    void VMTDController::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDControllerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    VMTDSettings *VMTDController::settings() const
    {
        return m_settings;
    }

    const QVector<VMTDNxApiAdapter *> &VMTDController::nxApiAdapters() const
    {
        return m_nxApiAdapters;
    }

    VMTDNodeServer *VMTDController::nodeServer() const
    {
        return m_nodeServer;
    }

    VMTDNodeClient *VMTDController::nodeClient() const
    {
        return m_nodeClient;
    }

    void VMTDController::startController()
    {
        start();
    }
    void VMTDController::stopController()
    {
        quit();
        wait();
    }

    void VMTDController::run()
    {
        if (m_settings->nodeType() == VMTDNodeType::CLIENT)
        {
            m_nodeClient = new VMTDNodeClient(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeClient, &VMTDNodeClient::deleteLater);
            m_nodeClient->connectSocketSlot();
        }
        else if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            m_nodeServer = new VMTDNodeServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeServer, &VMTDNodeServer::deleteLater);
            m_nodeServer->startListenSlot();

            m_netManager = new QNetworkAccessManager(nullptr);
            connect(this, &VMTDController::finished, m_netManager, &QNetworkAccessManager::deleteLater);

            auto jsonObj = m_settings->nxApiAdaptersParams();

            for (int i = 0; i < jsonObj.size(); ++i)
            {
                auto nxApiAdapter = new VMTDNxApiAdapter(nullptr, m_settings, m_netManager);
                connect(this, &VMTDController::finished, nxApiAdapter, &VMTDNxApiAdapter::deleteLater);
                m_nxApiAdapters.append(nxApiAdapter);

                nxApiAdapter->fromJson(jsonObj[QString("nxApiAdapter_%1").arg(i)].toObject());
            }
        }

        exec();

        if (m_settings->nodeType() == VMTDNodeType::CLIENT)
        {
            m_nodeClient->disconnectSocketSlot();
        }
        else if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            m_nodeServer->stopListenSlot();
        }

        m_nxApiAdapters.clear();
    }

    void VMTDController::startedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " started");
    }
    void VMTDController::finishedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " finished");
    }
}

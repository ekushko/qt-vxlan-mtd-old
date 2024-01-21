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
            m_nxApiServer = new VMTDNxApiServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nxApiServer, &VMTDNxApiServer::deleteLater);
            m_nxApiServer->startListenSlot();

            m_nodeServer = new VMTDNodeServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeServer, &VMTDNodeServer::deleteLater);
            m_nodeServer->startListenSlot();
        }

        exec();

        if (m_settings->nodeType() == VMTDNodeType::CLIENT)
        {
            m_nodeClient->disconnectSocketSlot();
        }
        else if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            m_nxApiServer->stopListenSlot();

            m_nodeServer->stopListenSlot();
        }
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

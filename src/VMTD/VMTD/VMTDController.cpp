#include "VMTDController.h"
#include "VMTDControllerForm.h"

#include "VMTDRepo.h"

#include <QCoreApplication>

namespace VMTDLib
{
    VMTDController::VMTDController(QObject *parent, const QString &systemName)
        : QThread(parent)
    {
        m_settings = new VMTDSettings(this, systemName);

        connect(this, &VMTDController::started, this, &VMTDController::startedSlot);
        connect(this, &VMTDController::finished, this, &VMTDController::finishedSlot);

        m_net = new VMTDNet(this, m_settings);

        m_settings->debugOut(VN_S(VMTDController) + " was created");
    }

    VMTDController::~VMTDController()
    {
        if (isRunning())
            stopController();

        if (m_form != nullptr)
            delete m_form;

        m_settings->debugOut(VN_S(VMTDController) + " was deleted");
    }

    VMTDSettings *VMTDController::settings() const
    {
        return m_settings;
    }

    VMTDNxApiServer *VMTDController::nxApiServer() const
    {
        return m_nxApiServer;
    }

    VMTDNodeServer *VMTDController::nodeServer() const
    {
        return m_nodeServer;
    }

    VMTDNodeClient *VMTDController::nodeClient() const
    {
        return m_nodeClient;
    }

    VMTDProtocol *VMTDController::protocol() const
    {
        return m_protocol;
    }

    VMTDNet *VMTDController::net() const
    {
        return m_net;
    }

    void VMTDController::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDControllerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
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
        m_protocol = new VMTDProtocol(nullptr, m_net);
        connect(this, &VMTDController::finished, m_protocol, &VMTDProtocol::deleteLater);

        if (m_settings->nodeType() == VMTDNodeType::CLIENT)
        {
            m_nodeClient = new VMTDNodeClient(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeClient, &VMTDNodeClient::deleteLater);
            m_protocol->setNodeClient(m_nodeClient);
            m_nodeClient->connectSocketSlot();
        }
        else if (m_settings->nodeType() == VMTDNodeType::SERVER)
        {
            m_nxApiServer = new VMTDNxApiServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nxApiServer, &VMTDNxApiServer::deleteLater);
            m_protocol->setNxApiServer(m_nxApiServer);
            m_nxApiServer->startListenSlot();

            m_nodeServer = new VMTDNodeServer(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeServer, &VMTDNodeServer::deleteLater);
            m_protocol->setNodeServer(m_nodeServer);
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

    void VMTDController::netApplySlot()
    {
        stopController();
        startController();
    }
}

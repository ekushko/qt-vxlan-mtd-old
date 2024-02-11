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
        connect(m_settings, &VMTDSettings::restartSignal, this, &VMTDController::restartSlot);

        m_settings->creationOut(VN_S(VMTDController) + " | Constructor called");

        connect(this, &VMTDController::started, this, &VMTDController::startedSlot);
        connect(this, &VMTDController::finished, this, &VMTDController::finishedSlot);

        m_deviceManager = new VMTDDeviceManager(this, m_settings);
        m_connectionManager = new VMTDConnectionManager(this, m_deviceManager);

        m_settings->creationOut(VN_S(VMTDController) + " | Constructor finished");
    }

    VMTDController::~VMTDController()
    {
        m_settings->creationOut(VN_S(VMTDController) + " | Destructor called");

        if (isRunning())
            stopController();

        if (m_form != nullptr)
            delete m_form;

        delete m_connectionManager;
        delete m_deviceManager;

        m_settings->creationOut(VN_S(VMTDController) + " | Destructor finished");

        delete m_settings;
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

    VMTDDeviceManager *VMTDController::deviceManager() const
    {
        return m_deviceManager;
    }

    VMTDConnectionManager *VMTDController::connectionManager() const
    {
        return m_connectionManager;
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
        m_settings->debugOut(VN_S(VMTDController) + " | Starting...");

        start();
    }
    void VMTDController::stopController()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Stopping...");

        quit();
        wait();
    }

    void VMTDController::run()
    {
        m_nodeType = m_settings->nodeType();

        m_protocol = new VMTDProtocol(nullptr, m_deviceManager);
        connect(this, &VMTDController::finished, m_protocol, &VMTDProtocol::deleteLater);

        if (m_nodeType == VMTDNodeType::CLIENT)
        {
            m_nodeClient = new VMTDNodeClient(nullptr, m_settings);
            connect(this, &VMTDController::finished, m_nodeClient, &VMTDNodeClient::deleteLater);
            m_protocol->setNodeClient(m_nodeClient);
            m_nodeClient->connectSocketSlot();
        }
        else if (m_nodeType == VMTDNodeType::SERVER)
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

        if (m_nodeType == VMTDNodeType::CLIENT)
        {
            m_nodeClient->disconnectSocketSlot();
        }
        else if (m_nodeType == VMTDNodeType::SERVER)
        {
            m_nxApiServer->stopListenSlot();

            m_nodeServer->stopListenSlot();
        }
    }

    void VMTDController::restartSlot()
    {
        const auto shouldBeStarted = isRunning();

        if (shouldBeStarted)
            stopController();

        if (shouldBeStarted)
            startController();
    }

    void VMTDController::startedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Started");
    }
    void VMTDController::finishedSlot()
    {
        m_settings->debugOut(VN_S(VMTDController) + " | Stopped");
    }
}

#include "VMTDProtocol.h"
#include "VMTDProtocolForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDProtocol::VMTDProtocol(QObject *parent, VMTDDeviceManager *deviceManager)
        : QObject(parent)
        , m_deviceManager(deviceManager)
        , m_settings(deviceManager->settings())
    {
        m_settings->creationOut(VN_S(VMTDProtocol) + " | Constructor called");

        m_checkConnectionTimer.setParent(this);
        m_checkConnectionTimer.setInterval(m_settings->checkConnectionInterval());
        connect(&m_checkConnectionTimer, &QTimer::timeout,
                this, &VMTDProtocol::checkConnectionTimerTickSlot);

        if (m_settings->nodeType() == VMTDNodeType::SERVER
            && m_settings->shouldCheckConnection())
            m_checkConnectionTimer.start();

        m_settings->creationOut(VN_S(VMTDProtocol) + " | Constructor finished");
    }

    VMTDProtocol::~VMTDProtocol()
    {
        m_settings->creationOut(VN_S(VMTDProtocol) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDProtocol) + " | Destructor finished");
    }

    VMTDSettings *VMTDProtocol::settings() const
    {
        return m_settings;
    }

    void VMTDProtocol::setNxApiServer(VMTDNxApiServer *nxApiServer)
    {
        m_nxApiServer = nxApiServer;

        connect(m_nxApiServer, &VMTDNxApiServer::adapterCreatedSignal,
                this, &VMTDProtocol::adapterCreatedSlot);
        connect(m_nxApiServer, &VMTDNxApiServer::adapterRemovedSignal,
                this, &VMTDProtocol::adapterRemovedSlot);
    }

    void VMTDProtocol::setNodeServer(VMTDNodeServer *nodeServer)
    {
        m_nodeServer = nodeServer;

        connect(m_nodeServer, &VMTDNodeServer::clientConnectedSignal,
                this, &VMTDProtocol::clientConnectedSlot);
        connect(m_nodeServer, &VMTDNodeServer::clientDisconnectedSignal,
                this, &VMTDProtocol::clientDisconnectedSlot);
    }

    void VMTDProtocol::setNodeClient(VMTDNodeClient *nodeClient)
    {
        m_nodeClient = nodeClient;

        connect(m_nodeClient, &VMTDNodeClient::socketConnectedSignal,
                this, &VMTDProtocol::socketConnectedSlot);
        connect(m_nodeClient, &VMTDNodeClient::socketDisconnectedSignal,
                this, &VMTDProtocol::socketDisconnectedSlot);
    }

    VMTDNodeProtocolHandler *VMTDProtocol::clientNodeHandler() const
    {
        return m_nodeHandler;
    }
    QList<VMTDNxApiProtocolHandler *> VMTDProtocol::nxApiHandlers() const
    {
        return m_nxApiHandlers.values();
    }
    QList<VMTDNodeProtocolHandler *> VMTDProtocol::nodeHandlers() const
    {
        return m_nodeHandlers.values();
    }
    QList<VMTDProtocolHandler *> VMTDProtocol::handlers() const
    {
        return m_handlers.values();
    }

    void VMTDProtocol::showFormSlot()
    {
        auto mainWidget = m_settings->mainWidget();

        if (m_form == nullptr)
            m_form = new VMTDProtocolForm(mainWidget, this);

        if (mainWidget != nullptr)
        {
            mainWidget->addWidget(m_form);
            mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDProtocol::adapterCreatedSlot(VMTDNxApiAdapter *adapter)
    {
        if (adapter == nullptr)
        {
            m_settings->debugOut(QString("%1 | NX-API Adapter is NULL!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        if (m_nxApiHandlers.contains(adapter))
        {
            m_settings->debugOut(QString("%1 | NX-API handler for adapter (%2) already created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(adapter->url().toString(QUrl::RemoveUserInfo)));
            return;
        }

        auto handler = new VMTDNxApiProtocolHandler(this, m_settings,
                                                    m_deviceManager->nxApiDevice(adapter->url()),
                                                    adapter);
        m_nxApiHandlers[adapter] = handler;
        m_handlers[handler->id()] = handler;

        connect(handler, &VMTDNxApiProtocolHandler::sendCommandSignal,
                adapter, &VMTDNxApiAdapter::sendCommandSlot);
        connect(adapter, &VMTDNxApiAdapter::commandExecutedSignal,
                handler, &VMTDNxApiProtocolHandler::commandExecutedSlot);

        emit handlerCreatedSignal(handler);

        m_settings->debugOut(VN_S(VMTDProtocol) + " | NX-API handler created!");
    }
    void VMTDProtocol::adapterRemovedSlot(VMTDNxApiAdapter *adapter)
    {
        if (!m_nxApiHandlers.contains(adapter))
        {
            m_settings->debugOut(QString("%1 | NX-API handler for adapter (%2) not created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(adapter->url().toString(QUrl::RemoveUserInfo)));
            return;
        }

        auto handler = m_nxApiHandlers[adapter];

        m_nxApiHandlers.remove(adapter);
        m_handlers.remove(handler->id());
        emit handlerRemovedSignal(handler);
        delete handler;

        m_settings->debugOut(VN_S(VMTDProtocol) + " | NX-API handler removed!");
    }

    void VMTDProtocol::clientConnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        if (m_nodeHandlers.contains(socket))
        {
            m_settings->debugOut(QString("%1 | Node handler for websocket (%2) already created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(socket->peerAddress().toString()));
            return;
        }

        const auto peerAddress = QHostAddress(socket->peerAddress().toIPv4Address()).toString();
        auto handler = new VMTDNodeProtocolHandler(this, m_settings, VMTDProtocolHandler::EnSide::SERVER,
                                                   m_deviceManager->nodeDevice(peerAddress),
                                                   socket);
        m_nodeHandlers[socket] = handler;
        m_handlers[handler->id()] = handler;

        connect(handler, &VMTDNodeProtocolHandler::sendMessageSignal,
                m_nodeServer, &VMTDNodeServer::sendMessageSlot);
        connect(m_nodeServer, &VMTDNodeServer::receiveMessageSignal,
                handler, &VMTDNodeProtocolHandler::receiveMessageSlot);

        emit handlerCreatedSignal(handler);

        m_settings->debugOut(VN_S(VMTDProtocol) + " | Node handler created!");
    }
    void VMTDProtocol::clientDisconnectedSlot(QWebSocket *socket)
    {
        if (!m_nodeHandlers.contains(socket))
        {
            m_settings->debugOut(QString("%1 | Node handler for websocket (%2) not created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(socket->peerAddress().toString()));
            return;
        }

        auto handler = m_nodeHandlers[socket];
        m_nodeHandlers.remove(socket);
        m_handlers.remove(handler->id());
        emit handlerRemovedSignal(handler);
        delete handler;

        m_settings->debugOut(VN_S(VMTDProtocol) + " | Node handler removed!");
    }

    void VMTDProtocol::socketConnectedSlot(QWebSocket *socket)
    {
        if (socket == nullptr)
        {
            m_settings->debugOut(QString("%1 | Websocket is NULL!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        if (m_socket != nullptr)
        {
            m_settings->debugOut(QString("%1 | Node handler already created!")
                                 .arg(VN_S(VMTDProtocol)));
            return;
        }

        m_socket = socket;

        m_nodeHandler = new VMTDNodeProtocolHandler(this, m_settings, VMTDProtocolHandler::EnSide::CLIENT,
                                                    m_deviceManager->nodeDevice(m_settings->serverIp()),
                                                    m_socket);

        connect(m_nodeHandler, &VMTDNodeProtocolHandler::sendMessageSignal,
                m_nodeClient, &VMTDNodeClient::sendDataSlot);
        connect(m_nodeClient, &VMTDNodeClient::receiveMessageSignal,
                m_nodeHandler, &VMTDNodeProtocolHandler::receiveMessageSlot);
        connect(m_nodeHandler, &VMTDNodeProtocolHandler::handleMethodSignal,
                this, &VMTDProtocol::handleMethodSignal,
                Qt::DirectConnection);

        emit handlerCreatedSignal(m_nodeHandler);

        m_settings->debugOut(VN_S(VMTDProtocol) + " | Node handler created!");
    }
    void VMTDProtocol::socketDisconnectedSlot(QWebSocket *socket)
    {
        if (m_socket != socket)
        {
            m_settings->debugOut(QString("%1 | Node handler for websocket (%2) not created!")
                                 .arg(VN_S(VMTDProtocol))
                                 .arg(socket->peerAddress().toString()));
            return;
        }

        m_socket = nullptr;

        delete m_nodeHandler;

        m_settings->debugOut(VN_S(VMTDProtocol) + " | Node handler removed!");
    }

    void VMTDProtocol::checkConnectionTimerTickSlot()
    {
        for (auto handler : m_handlers)
            handler->checkConnection();
    }
}

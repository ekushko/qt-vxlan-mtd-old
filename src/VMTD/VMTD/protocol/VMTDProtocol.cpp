#include "VMTDProtocol.h"
#include "VMTDProtocolForm.h"

namespace VMTDLib
{
    VMTDProtocol::VMTDProtocol(QObject *parent, VMTDNet *model)
        : QObject(parent)
        , m_model(model)
        , m_settings(model->settings())
    {
        connect(m_settings, &VMTDSettings::checkConnectionChangedSignal,
                this, &VMTDProtocol::checkConnectionChangedSlot);

        m_checkConnectionTimer.setParent(this);
        connect(&m_checkConnectionTimer, &QTimer::timeout,
                this, &VMTDProtocol::checkConnectionTimerTickSlot);
        checkConnectionChangedSlot();
    }

    VMTDProtocol::~VMTDProtocol()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
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

    VMTDProtocolHandler *VMTDProtocol::handler(const QString &name) const
    {
        return m_handlers.value(name);
    }
    QList<VMTDProtocolHandler *> VMTDProtocol::handlers() const
    {
        return m_handlers.values();
    }

    void VMTDProtocol::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDProtocolForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDProtocol::adapterCreatedSlot(VMTDNxApiAdapter *adapter)
    {
        if (m_nxApiHandlers.contains(adapter))
            return;

        auto nxApiDevice = m_model->nxApiDevice(adapter->url());

        if (nxApiDevice == nullptr)
            return;

        auto handler = new VMTDNxApiProtocolHandler(this, m_settings, nxApiDevice, adapter);
        m_nxApiHandlers[adapter] = handler;
        m_handlers[handler->name()] = handler;

        connect(handler, &VMTDNxApiProtocolHandler::sendCommandSignal,
                adapter, &VMTDNxApiAdapter::sendCommandSlot);
        connect(adapter, &VMTDNxApiAdapter::commandExecutedSignal,
                handler, &VMTDNxApiProtocolHandler::commandExecutedSlot);

        emit handlerCreatedSignal(handler);
    }
    void VMTDProtocol::adapterRemovedSlot(VMTDNxApiAdapter *adapter)
    {
        if (!m_nxApiHandlers.contains(adapter))
            return;

        auto handler = m_nxApiHandlers[adapter];

        emit handlerRemovedSignal(handler);

        m_nxApiHandlers.remove(adapter);
        m_handlers.remove(handler->name());
        delete handler;
    }

    void VMTDProtocol::clientConnectedSlot(QWebSocket *socket)
    {
        if (m_nodeHandlers.contains(socket))
            return;

        auto nodeDevice = m_model->nodeDevice(socket->peerAddress().toString());

        if (nodeDevice == nullptr)
            return;

        auto handler = new VMTDNodeProtocolHandler(this, m_settings, nodeDevice, socket);
        m_nodeHandlers[socket] = handler;
        m_handlers[handler->name()] = handler;

        connect(handler, &VMTDNodeProtocolHandler::sendMessageSignal,
                m_nodeServer, &VMTDNodeServer::sendMessageSlot);
        connect(m_nodeServer, &VMTDNodeServer::receiveMessageSignal,
                handler, &VMTDNodeProtocolHandler::receiveMessageSlot);

        emit handlerCreatedSignal(handler);
    }
    void VMTDProtocol::clientDisconnectedSlot(QWebSocket *socket)
    {
        if (!m_nodeHandlers.contains(socket))
            return;

        auto handler = m_nodeHandlers[socket];

        emit handlerRemovedSignal(handler);

        m_handlers.remove(handler->name());
        m_nodeHandlers.remove(socket);
        delete handler;
    }

    void VMTDProtocol::socketConnectedSlot(QWebSocket *socket)
    {
        if (m_socket != nullptr)
            return;

        m_socket = socket;

        auto nodeDevice = m_model->nodeDevice(m_socket->peerAddress().toString());

        m_nodeHandler = new VMTDNodeProtocolHandler(this, m_settings, nodeDevice, m_socket);
        m_handlers[m_nodeHandler->name()] = m_nodeHandler;

        connect(m_nodeHandler, &VMTDNodeProtocolHandler::sendMessageSignal,
                m_nodeClient, &VMTDNodeClient::sendDataSlot);
        connect(m_nodeClient, &VMTDNodeClient::receiveMessageSignal,
                m_nodeHandler, &VMTDNodeProtocolHandler::receiveMessageSlot);

        emit handlerCreatedSignal(m_nodeHandler);
    }
    void VMTDProtocol::socketDisconnectedSlot(QWebSocket *socket)
    {
        if (m_socket != socket)
            return;

        m_socket = nullptr;

        m_handlers.remove(m_nodeHandler->name());
        delete m_nodeHandler;
    }

    void VMTDProtocol::checkConnectionChangedSlot()
    {
        if (m_checkConnectionTimer.isActive())
            m_checkConnectionTimer.stop();

        m_checkConnectionTimer.setInterval(m_settings->checkConnectionInterval());

        if (m_settings->shouldCheckConnection())
            m_checkConnectionTimer.start();
    }

    void VMTDProtocol::checkConnectionTimerTickSlot()
    {
        for (auto handler : m_handlers)
            handler->checkConnection();
    }
}

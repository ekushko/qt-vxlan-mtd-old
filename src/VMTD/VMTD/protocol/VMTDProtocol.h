#pragma once

#include "../adapter/nxapi/VMTDNxApiServer.h"
#include "../adapter/node/VMTDNodeServer.h"
#include "../adapter/node/VMTDNodeClient.h"

#include "nxapi/VMTDNxApiProtocolHandler.h"
#include "node/VMTDNodeProtocolHandler.h"

namespace VMTDLib
{
    class VMTDProtocolForm;

    class VMTDProtocol : public QObject
    {
        Q_OBJECT

    public:

        VMTDProtocol(QObject *parent, VMTDDeviceManager *deviceManager);
        ~VMTDProtocol();

        VMTDSettings *settings() const;

        void setNxApiServer(VMTDNxApiServer *nxApiServer);

        void setNodeServer(VMTDNodeServer *nodeServer);

        void setNodeClient(VMTDNodeClient *nodeClient);

        QList<VMTDNxApiProtocolHandler *> nxApiHandlers() const;
        QList<VMTDNodeProtocolHandler *> nodeHandlers() const;
        QList<VMTDProtocolHandler *> handlers() const;

    signals:

        void handleMethodSignal(const QString &method, const QJsonObject &params, bool &result);

        void handlerCreatedSignal(VMTDProtocolHandler *handler);
        void handlerRemovedSignal(VMTDProtocolHandler *handler);

    public slots:

        void showFormSlot(QWidget *parent);

    private:

        QPointer<VMTDProtocolForm> m_form;

        VMTDDeviceManager *m_deviceManager;

        VMTDSettings *m_settings;

        VMTDNxApiServer *m_nxApiServer;
        QMap<VMTDNxApiAdapter *, VMTDNxApiProtocolHandler *> m_nxApiHandlers;

        VMTDNodeServer *m_nodeServer;
        QMap<QWebSocket *, VMTDNodeProtocolHandler *> m_nodeHandlers;

        QWebSocket *m_socket;
        VMTDNodeClient *m_nodeClient;
        VMTDNodeProtocolHandler *m_nodeHandler;

        QMap<int, VMTDProtocolHandler *> m_handlers;
        QTimer m_checkConnectionTimer;

    private slots:

        void adapterCreatedSlot(VMTDNxApiAdapter *adapter);
        void adapterRemovedSlot(VMTDNxApiAdapter *adapter);

        void clientConnectedSlot(QWebSocket *socket);
        void clientDisconnectedSlot(QWebSocket *socket);

        void socketConnectedSlot(QWebSocket *socket);
        void socketDisconnectedSlot(QWebSocket *socket);

        void checkConnectionTimerTickSlot();
    };
}

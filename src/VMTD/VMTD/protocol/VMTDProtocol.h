#pragma once

#include "../adapter/nxapi/VMTDNxApiServer.h"
#include "../adapter/node/VMTDNodeServer.h"
#include "../adapter/node/VMTDNodeClient.h"

#include "VMTDProtocolNxApiHandler.h"
#include "VMTDProtocolNodeHandler.h"

#include <QTimer>

namespace VMTDLib
{
    class VMTDProtocolForm;

    class VMTDProtocol : public QObject
    {
        Q_OBJECT

    public:

        VMTDProtocol(QObject *parent, VMTDModel *model);
        ~VMTDProtocol();

        void setNxApiServer(VMTDNxApiServer *nxApiServer);

        void setNodeServer(VMTDNodeServer *nodeServer);

        void setNodeClient(VMTDNodeClient *nodeClient);

        const QVector<VMTDProtocolHandler *> &handlers() const;

    signals:

        void handlerCreatedSignal(VMTDProtocolHandler *handler);
        void handlerRemovedSignal(VMTDProtocolHandler *handler);

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDProtocolForm> m_form;

        VMTDModel *m_model;

        VMTDSettings *m_settings;

        VMTDNxApiServer *m_nxApiServer;
        QMap<VMTDNxApiAdapter *, VMTDProtocolNxApiHandler *> m_nxApiHandlers;

        VMTDNodeServer *m_nodeServer;
        QMap<QWebSocket *, VMTDProtocolNodeHandler *> m_nodeHandlers;

        QWebSocket *m_socket;
        VMTDNodeClient *m_nodeClient;
        VMTDProtocolNodeHandler *m_nodeHandler;

        QVector<VMTDProtocolHandler *> m_handlers;
        QTimer m_checkConnectionTimer;

    private slots:

        void adapterCreatedSlot(VMTDNxApiAdapter *adapter);
        void adapterRemovedSlot(VMTDNxApiAdapter *adapter);

        void clientConnectedSlot(QWebSocket *socket);
        void clientDisconnectedSlot(QWebSocket *socket);

        void socketConnectedSlot(QWebSocket *socket);
        void socketDisconnectedSlot(QWebSocket *socket);

        void checkConnectionChangedSlot();
        void checkConnectionTimerTickSlot();
    };
}

#pragma once

#include "../VMTDProtocolHandler.h"

#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace VMTDLib
{
    class VMTDNodeProtocolHandlerForm;

    class VMTDNodeProtocolHandler : public VMTDProtocolHandler
    {
        Q_OBJECT

    public:

        VMTDNodeProtocolHandler(QObject *parent,
                                VMTDSettings *settings, EnSide side,
                                VMTDNodeDevice *device,
                                QWebSocket *socket);
        ~VMTDNodeProtocolHandler();

        // КОНСТАНТЫ

        enum class EnError
        {
            // -32000..32099 резерв
            PARSE_ERROR      = -32700,
            INVALID_REQUEST  = -32600,
            METHOD_NOT_FOUND = -32601,
            INVALID_PARAMS   = -32602,
            INTERNAL_ERROR   = -32603
        };
        Q_ENUM(EnError)
        static const QString            &enErrorToS(const EnError &error);
        static const QMap<int, QString> &enErrorToL();

        // ЛОГИКА

        void checkConnection() override;

        // ДАННЫЕ

        QString name() const override;

        int queueLength() const override;

    signals:

        void sendMessageSignal(QWebSocket *socket, const QString &data);

        void handleMethodSignal(const QString &method, const QJsonObject &params, bool &result);

    public slots:

        void showFormSlot() override;

        void appendRequestsSlot(const QList<QPair<QString, QJsonObject>> &requests);

        void receiveMessageSlot(QWebSocket *socket, const QString &data);

        void clearQueueSlot() override;

    private:

        bool           isRequest(const QJsonObject &request) const;
        QJsonObject buildRequest(const QString &method, const QJsonObject &params);

        bool           isResponse(const QJsonObject &response) const;
        QJsonObject buildResponse(const QJsonValue &id, const QJsonValue &result);
        QJsonObject buildError(const QJsonValue &id, int code, const QString &message);

        void handleServer(const QJsonObject &response);
        void handleClient(const QJsonObject &request, QJsonObject &response);

        QPointer<VMTDNodeProtocolHandlerForm> m_form;

        QPointer<VMTDNodeDevice> m_device;

        QWebSocket *m_socket;

        int m_commandCounter = 0;

        QQueue<QJsonDocument> m_messages;

    private slots:

        void checkQueueTimerSlot() override;

        void ticketTimeoutSlot() override;
    };
}

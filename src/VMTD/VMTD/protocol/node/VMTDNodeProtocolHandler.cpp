#include "VMTDNodeProtocolHandler.h"
#include "VMTDNodeProtocolHandlerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeProtocolHandler::VMTDNodeProtocolHandler(QObject *parent, VMTDSettings *settings,
                                                     VMTDNodeDevice *device,
                                                     QWebSocket *socket)
        : VMTDProtocolHandler(parent, settings, EnType::NODE)
        , m_device(device)
        , m_socket(socket)
    {
        m_ticketTimeoutTimer.setInterval(m_device->ticketTimeoutInterval());

        m_checkQueueTimer.setInterval(m_device->checkQueueInterval());
        m_checkQueueTimer.start();
    }

    VMTDNodeProtocolHandler::~VMTDNodeProtocolHandler()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    const QString &VMTDNodeProtocolHandler::enErrorToS(const EnError &error)
    {
        switch (error)
        {
        // *INDENT-OFF*
        case EnError::PARSE_ERROR      : RETURN_S("Parse error");
        case EnError::INVALID_REQUEST  : RETURN_S("Invalid request");
        case EnError::METHOD_NOT_FOUND : RETURN_S("Method not found");
        case EnError::INVALID_PARAMS   : RETURN_S("Invalid params");
        case EnError::INTERNAL_ERROR   : RETURN_S("Internal error");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDNodeProtocolHandler::enErrorToL()
    {
        RETURN_MAP(EnError, enErrorToS);
    }

    void VMTDNodeProtocolHandler::checkConnection()
    {
        // в разработке
    }

    QString VMTDNodeProtocolHandler::name() const
    {
        return QString("id: %1 [%2]")
               .arg(m_device->id())
               .arg(m_device->ip());
    }

    int VMTDNodeProtocolHandler::queueLength() const
    {
        return m_messages.length();
    }

    void VMTDNodeProtocolHandler::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeProtocolHandlerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNodeProtocolHandler::appendRequestSlot(const QString &method, const QJsonObject &params)
    {
        m_messages.append(buildRequest(method, params));
    }

    void VMTDNodeProtocolHandler::receiveMessageSlot(QWebSocket *socket, const QByteArray &data)
    {
        QJsonParseError parseError;
        QJsonDocument inputDoc, outputDoc;

        inputDoc = QJsonDocument::fromJson(data, &parseError);

        emit showDebugSignal(QTime::currentTime(), QString("Message received:\n")
                             + inputDoc.toJson(QJsonDocument::JsonFormat::Indented));

        if (parseError.error != QJsonParseError::NoError
            || inputDoc.isEmpty())
        {
            outputDoc.setObject(buildError(QJsonValue(),
                                           (int)EnError::PARSE_ERROR,
                                           enErrorToS(EnError::PARSE_ERROR)));
        }
        else
        {
            if (inputDoc.isObject())
            {
                const auto jsonObj = handleMessage(inputDoc.object());

                if (!jsonObj.isEmpty())
                    outputDoc.setObject(jsonObj);
            }
            else if (inputDoc.isArray())
            {
                QJsonArray jsonArr;

                for (int i = 0; i < inputDoc.array().size(); ++i)
                {
                    const auto jsonObj = handleMessage(inputDoc.array().at(i).toObject());

                    if (!jsonObj.isEmpty())
                        jsonArr.append(jsonObj);
                }

                if (!jsonArr.isEmpty())
                    outputDoc.setArray(jsonArr);
            }
        }

        if (!outputDoc.isEmpty())
        {
            emit sendMessageSignal(socket, outputDoc.toBinaryData());

            emit showDebugSignal(QTime::currentTime(), QString("Message sent:\n")
                                 + outputDoc.toJson(QJsonDocument::JsonFormat::Indented));
        }
    }

    void VMTDNodeProtocolHandler::clearQueueSlot()
    {
        m_messages.clear();
    }

    bool VMTDNodeProtocolHandler::isRequest(const QJsonObject &request) const
    {
        bool isValid = true;

        isValid &= request.size() >= 2;
        isValid &= request.contains("jsonrpc") && request["jsonrpc"].toString() == "2.0";
        isValid &= request.contains("method") && !request["method"].isNull();

        return isValid;
    }
    QJsonObject VMTDNodeProtocolHandler::buildRequest(const QString &method, const QJsonObject &params)
    {
        QJsonObject jsonObj;

        jsonObj["jsonrpc"] = "2.0";
        jsonObj["method"] = method;
        jsonObj["params"] = params;
        jsonObj["id"] = QString("%1").arg(m_commandCounter++);

        return jsonObj;
    }

    bool VMTDNodeProtocolHandler::isResponse(const QJsonObject &response) const
    {
        bool isValid = true;

        isValid &= response.size() >= 2;
        isValid &= response.contains("jsonrpc") && response["jsonrpc"].toString() == "2.0";
        isValid &= response.contains("result") || response.contains("error");

        return isValid;
    }
    QJsonObject VMTDNodeProtocolHandler::buildResponse(const QJsonValue &id,
                                                       const QJsonValue &result)
    {
        QJsonObject jsonObj;

        jsonObj["result"] = result;
        jsonObj["id"] = id;

        return jsonObj;
    }
    QJsonObject VMTDNodeProtocolHandler::buildError(const QJsonValue &id,
                                                    int code, const QString &message)
    {
        QJsonObject jsonObj, errorObj;

        errorObj["code"] = code;
        errorObj["message"] = message;

        jsonObj["error"] = errorObj;
        jsonObj["id"] = id;

        return jsonObj;
    }

    bool VMTDNodeProtocolHandler::isMethodExist(const QString &method, const QJsonValue &params)
    {
        Q_UNUSED(method);
        Q_UNUSED(params);

        return false;
    }

    QJsonObject VMTDNodeProtocolHandler::handleMessage(const QJsonObject &message)
    {
        QJsonObject jsonObj;

        if (isRequest(message))
        {
            if (!isMethodExist(message["method"].toString(), message["params"]))
            {
                jsonObj = buildError(message["id"],
                                     (int)EnError::METHOD_NOT_FOUND,
                                     enErrorToS(EnError::METHOD_NOT_FOUND));
            }
            else
            {
                // в разработке
            }
        }
        else if (isResponse(message))
        {
            if (m_currentMessage["id"].toString() == message["id"].toString())
            {
                if (m_ticketTimeoutTimer.isActive())
                    m_ticketTimeoutTimer.stop();

                m_device->setOnline(true);

                if (message.contains("error"))
                {
                    emit showDebugSignal(QTime::currentTime(), QString("Error: %1")
                                         .arg(message["error"].toString()));
                }
                else if (message.contains("result"))
                {
                    // в разработке
                }

                m_currentMessage = QJsonObject();
                m_queueState = EnQueueState::READY_TO_SEND;
            }
        }
        else
        {
            jsonObj = buildError(message["id"],
                                 (int)EnError::INVALID_REQUEST,
                                 enErrorToS(EnError::INVALID_REQUEST));
        }

        return jsonObj;
    }


    void VMTDNodeProtocolHandler::checkQueueTimerSlot()
    {
        if (m_messages.isEmpty()
            || m_queueState != EnQueueState::READY_TO_SEND)
            return;

        m_queueState = EnQueueState::WAIT_FOR_TICKET;

        m_currentMessage = m_messages.dequeue();

        QJsonDocument outputDoc;
        outputDoc.setObject(m_currentMessage);

        emit showDebugSignal(QTime::currentTime(), QString("Message sent:\n")
                             + outputDoc.toJson(QJsonDocument::JsonFormat::Indented));
        emit sendMessageSignal(m_socket, outputDoc.toBinaryData());

        m_ticketTimeoutTimer.start();
    }

    void VMTDNodeProtocolHandler::ticketTimeoutSlot()
    {
        m_currentMessage = QJsonObject();
        m_queueState = EnQueueState::READY_TO_SEND;

        m_device->setOnline(false);

        emit showDebugSignal(QTime::currentTime(), "Response not received");
    }
}

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
        m_settings->debugOut(VN_S(VMTDNodeProtocolHandler) + " | Constructor called");

        // do nothing

        m_settings->debugOut(VN_S(VMTDNodeProtocolHandler) + " | Constructor finished");
    }

    VMTDNodeProtocolHandler::~VMTDNodeProtocolHandler()
    {
        m_settings->debugOut(VN_S(VMTDNodeProtocolHandler) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->debugOut(VN_S(VMTDNodeProtocolHandler) + " | Destructor finished");
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
               .arg(id())
               .arg(m_socket->peerAddress().toString());
    }

    int VMTDNodeProtocolHandler::queueLength() const
    {
        return m_messages.length();
    }

    void VMTDNodeProtocolHandler::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDNodeProtocolHandlerForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNodeProtocolHandler::appendRequestSlot(const QString &method, const QJsonObject &params)
    {
        if (m_settings->nodeType() == VMTDNodeType::SERVER)
            m_messages.append(buildRequest(method, params));
    }

    void VMTDNodeProtocolHandler::receiveMessageSlot(QWebSocket *socket, const QString &data)
    {
        if (data.isEmpty())
            return;

        QJsonParseError parseError;
        QJsonDocument inputDoc, outputDoc;

        inputDoc = QJsonDocument::fromJson(data.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError)
        {
            if (m_settings->nodeType() == VMTDNodeType::CLIENT)
            {
                emit showDebugSignal(QTime::currentTime(), QString("Parsing request error (%1): %2\n")
                                     .arg(parseError.error)
                                     .arg(parseError.errorString()));

                outputDoc.setObject(buildError(QJsonValue(),
                                               (int)EnError::PARSE_ERROR,
                                               enErrorToS(EnError::PARSE_ERROR)));
            }
            else if (m_settings->nodeType() == VMTDNodeType::SERVER)
            {
                emit showDebugSignal(QTime::currentTime(), QString("Parsing response error: %1\n")
                                     .arg(parseError.errorString()));
            }
        }
        else
        {
            if (m_settings->nodeType() == VMTDNodeType::CLIENT)
            {
                emit showDebugSignal(QTime::currentTime(), QString("Request received:\n")
                                     + inputDoc.toJson(QJsonDocument::JsonFormat::Indented));
            }
            else if (m_settings->nodeType() == VMTDNodeType::SERVER)
            {
                emit showDebugSignal(QTime::currentTime(), QString("Response received:\n")
                                     + inputDoc.toJson(QJsonDocument::JsonFormat::Indented));
            }

            if (inputDoc.isObject())
            {
                if (m_settings->nodeType() == VMTDNodeType::CLIENT)
                {
                    QJsonObject jsonObj;

                    handleClient(inputDoc.object(), jsonObj);

                    if (!jsonObj.isEmpty())
                        outputDoc.setObject(jsonObj);
                }
                else if (m_settings->nodeType() == VMTDNodeType::SERVER)
                {
                    handleServer(inputDoc.object());
                }

            }
            else if (inputDoc.isArray())
            {
                QJsonArray jsonArr;

                for (int i = 0; i < inputDoc.array().size(); ++i)
                {
                    QJsonObject jsonObj;

                    if (m_settings->nodeType() == VMTDNodeType::CLIENT)
                    {
                        handleClient(inputDoc.array().at(i).toObject(), jsonObj);

                        if (!jsonObj.isEmpty())
                            jsonArr.append(jsonObj);
                    }
                    else if (m_settings->nodeType() == VMTDNodeType::SERVER)
                    {
                        handleServer(inputDoc.array().at(i).toObject());
                    }
                }

                if (m_settings->nodeType() == VMTDNodeType::CLIENT && !jsonArr.isEmpty())
                    outputDoc.setArray(jsonArr);
            }
        }

        if (m_settings->nodeType() == VMTDNodeType::CLIENT && !outputDoc.isEmpty())
        {
            emit sendMessageSignal(socket, outputDoc.toJson(QJsonDocument::JsonFormat::Indented));

            emit showDebugSignal(QTime::currentTime(), QString("Response sent:\n")
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

        if (isValid && response.contains("error") && response["error"].isObject())
        {
            const auto errorObj = response["error"].toObject();

            isValid &= errorObj.contains("code");
            isValid &= !errorObj["code"].isNull();

            isValid &= errorObj.contains("message");
            isValid &= errorObj["message"].isString();
        }
        else
        {
            return false;
        }

        return isValid;
    }
    QJsonObject VMTDNodeProtocolHandler::buildResponse(const QJsonValue &id,
                                                       const QJsonValue &result)
    {
        QJsonObject jsonObj;

        jsonObj["jsonrpc"] = "2.0";
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

        jsonObj["jsonrpc"] = "2.0";
        jsonObj["error"] = errorObj;
        jsonObj["id"] = id;

        return jsonObj;
    }

    void VMTDNodeProtocolHandler::handleServer(const QJsonObject &response)
    {
        if (m_device != nullptr)
            m_device->setOnline(true);

        if (!isResponse(response))
        {
            emit showDebugSignal(QTime::currentTime(), QString("Handling response error: Invalid response\n"));
        }
        else
        {
            if (response.contains("error"))
            {
                const auto errorObj = response["error"].toObject();

                emit showDebugSignal(QTime::currentTime(), QString("Response error: %1\n")
                                     .arg(errorObj["message"].toString()));
            }
            else if (response.contains("result"))
            {
                // в разработке
            }
        }
    }
    void VMTDNodeProtocolHandler::handleClient(const QJsonObject &request, QJsonObject &response)
    {
        if (!isRequest(request))
        {
            const auto error = EnError::INVALID_REQUEST;

            emit showDebugSignal(QTime::currentTime(), QString("Handling request error: %1\n")
                                 .arg(enErrorToS(error)));

            response = buildError(request["id"], (int)error, enErrorToS(error));
        }
        else
        {
            // в разработке

            const auto error = EnError::METHOD_NOT_FOUND;

            emit showDebugSignal(QTime::currentTime(), QString("Handling request error: %1\n")
                                 .arg(enErrorToS(error)));

            response = buildError(request["id"], (int)error, enErrorToS(error));
        }
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
        emit sendMessageSignal(m_socket, outputDoc.toJson(QJsonDocument::JsonFormat::Indented));

        m_ticketTimeoutTimer.start();
    }

    void VMTDNodeProtocolHandler::ticketTimeoutSlot()
    {
        m_currentMessage = QJsonObject();
        m_queueState = EnQueueState::READY_TO_SEND;

        if (m_device != nullptr)
            m_device->setOnline(false);

        emit showDebugSignal(QTime::currentTime(), "Response not received");
    }
}

#include "VMTDNxApiAdapter.h"
#include "VMTDNxApiAdapterForm.h"

#include "../VMTDRepo.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNxApiAdapter::VMTDNxApiAdapter(QObject *parent,
                                       VMTDSettings *settings,
                                       QNetworkAccessManager *netManager)
        : QObject{parent}
        , m_settings{settings}
        , m_netManager{netManager}
        , m_url{"https://127.0.0.1/ins"}
        , m_ticketTimeoutInterval{10000}
    {
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + " was created");

        connect(m_netManager, &QNetworkAccessManager::finished,
                this, &VMTDNxApiAdapter::replyFinishedSlot);

        m_isConnected = false;
        m_ticketTimeout.setParent(this);
        m_ticketTimeout.setSingleShot(true);
        connect(&m_ticketTimeout, &QTimer::timeout, this, &VMTDNxApiAdapter::ticketTimeoutSlot);
    }

    VMTDNxApiAdapter::~VMTDNxApiAdapter()
    {
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + QString("(%1) was deleted").arg(m_url.toString()));

        if (m_form != nullptr)
            m_form->deleteLater();
    }

    void VMTDNxApiAdapter::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiAdapterForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    QJsonObject VMTDNxApiAdapter::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_url)] = m_url.toString();
        jsonObj["userName"] = m_url.userName();
        jsonObj["password"] = m_url.password();
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;

        return jsonObj;
    }
    void VMTDNxApiAdapter::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                .toInt(m_ticketTimeoutInterval);
    }

    VMTDSettings *VMTDNxApiAdapter::settings() const
    {
        return m_settings;
    }

    bool VMTDNxApiAdapter::isConnected() const
    {
        return m_isConnected;
    }

    bool VMTDNxApiAdapter::canSend() const
    {
        return !m_ticketTimeout.isActive();
    }

    QUrl VMTDNxApiAdapter::url() const
    {
        return m_url;
    }
    void VMTDNxApiAdapter::setUrl(const QUrl &url)
    {
        m_url = url;
    }

    int VMTDNxApiAdapter::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDNxApiAdapter::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        m_ticketTimeoutInterval = ticketTimeoutInterval;
    }

    void VMTDNxApiAdapter::checkConnectionSlot()
    {
        if (!canSend())
            return;

        QNetworkRequest request(m_url);
        buildRequest(&request);
        showRequest(&request);

        m_ticketTimeout.start(m_ticketTimeoutInterval);

        m_netManager->get(request);
    }

    void VMTDNxApiAdapter::sendCommandSlot(const QStringList &commands)
    {
        if (!canSend())
            return;

        QJsonArray jsonArray;

        for (const auto& command : commands)
        {
            QJsonObject jsonObj, paramsObj;

            jsonObj["jsonrpc"] = "2.0";
            jsonObj["method"] = "cli_ascii";

            paramsObj["cmd"] = command;
            paramsObj["version"] = 1;

            jsonObj["params"] = paramsObj;
            jsonObj["id"] = m_commandCounter++;

            jsonArray.append(jsonObj);
        }

        QJsonDocument jsonDoc;
        jsonDoc.setArray(jsonArray);

        QNetworkRequest request(m_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json-rpc");
        buildRequest(&request);
        showRequest(&request, &jsonDoc);

        m_ticketTimeout.start(m_ticketTimeoutInterval);

        m_netManager->post(request, jsonDoc.toJson());
    }

    void VMTDNxApiAdapter::replyFinishedSlot(QNetworkReply *reply)
    {
        if (!reply->isFinished())
            return;

        QString message = "Response:";

        if (reply->error() == QNetworkReply::NoError)
            message.append(reply->readAll());
        else
            message.append(reply->errorString());

        emit showMessageSignal(message);

        m_ticketTimeout.stop();
        m_isConnected = true;

        reply->deleteLater();
    }

    void VMTDNxApiAdapter::buildRequest(QNetworkRequest *request)
    {
        auto conf = request->sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request->setSslConfiguration(conf);
    }
    void VMTDNxApiAdapter::showRequest(QNetworkRequest *request, QJsonDocument *jsonDoc)
    {
        QString message = "Request:\n" + request->url().toString();

        const QList<QByteArray> &rawHeaderList(request->rawHeaderList());

        foreach (QByteArray rawHeader, rawHeaderList)
            message.append("\n" + request->rawHeader(rawHeader));

        if (jsonDoc != nullptr)
            message.append("\n" + jsonDoc->toJson(QJsonDocument::JsonFormat::Indented));

        emit showMessageSignal(message);
    }

    void VMTDNxApiAdapter::ticketTimeoutSlot()
    {
        QString message = "No response";

        emit showMessageSignal(message);

        m_isConnected = false;
    }
}

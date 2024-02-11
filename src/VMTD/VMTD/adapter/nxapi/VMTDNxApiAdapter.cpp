#include "VMTDNxApiAdapter.h"
#include "VMTDNxApiAdapterForm.h"

#include "../../VMTDRepo.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNxApiAdapter::VMTDNxApiAdapter(QObject *parent,
                                       VMTDSettings *settings,
                                       QNetworkAccessManager *netManager)
        : QObject(parent)
        , m_settings(settings)
        , m_netManager(netManager)
        , m_url("https://127.0.0.1/ins")
    {
        m_settings->creationOut(VN_S(VMTDNxApiAdapter) + " | Constructor called");

        connect(m_netManager, &QNetworkAccessManager::finished,
                this, &VMTDNxApiAdapter::replyFinishedSlot);

        m_settings->creationOut(VN_S(VMTDNxApiAdapter) + " | Constructor finished");
    }

    VMTDNxApiAdapter::~VMTDNxApiAdapter()
    {
        m_settings->creationOut(VN_S(VMTDNxApiAdapter) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->creationOut(VN_S(VMTDNxApiAdapter) + " | Destructor finished");
    }

    QJsonObject VMTDNxApiAdapter::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_url)] = m_url.toString();
        jsonObj["userName"] = m_url.userName();
        jsonObj["password"] = m_url.password();

        return jsonObj;
    }
    void VMTDNxApiAdapter::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_url = QUrl(jsonObj[VN_ME(m_url)].toString(m_url.toString()));
        m_url.setUserName(jsonObj["userName"].toString(m_url.userName()));
        m_url.setPassword(jsonObj["password"].toString(m_url.password()));
    }

    VMTDSettings *VMTDNxApiAdapter::settings() const
    {
        return m_settings;
    }

    QUrl VMTDNxApiAdapter::url() const
    {
        return m_url;
    }
    void VMTDNxApiAdapter::setUrl(const QUrl &url)
    {
        m_url = url;
    }

    void VMTDNxApiAdapter::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiAdapterForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNxApiAdapter::checkConnectionSlot()
    {
        QNetworkRequest request(m_url);
        buildRequest(&request);
        showRequest(&request);

        m_netManager->get(request);
    }

    void VMTDNxApiAdapter::sendCommandSlot(const QStringList &command)
    {
        QJsonArray jsonArray;

        for (const auto &command : command)
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

        m_netManager->post(request, jsonDoc.toJson());
    }

    void VMTDNxApiAdapter::replyFinishedSlot(QNetworkReply *reply)
    {
        if (!reply->isFinished())
            return;

        QString debugString = "Response received:\n";

        if (reply->error() == QNetworkReply::NoError)
            debugString.append(reply->readAll());
        else
            debugString.append(reply->errorString());

        emit commandExecutedSignal(reply->error() != QNetworkReply::ConnectionRefusedError,
                                   reply->error() != QNetworkReply::NoError);

        emit showDebugSignal(QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + QString("(%1) | %2")
                             .arg(m_url.toString(QUrl::RemoveUserInfo))
                             .arg(debugString));

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
        QString debugString = "Request sent:\n" + request->url().toString();

        const QList<QByteArray> &rawHeaderList(request->rawHeaderList());

        foreach (QByteArray rawHeader, rawHeaderList)
            debugString.append("\n" + request->rawHeader(rawHeader));

        if (jsonDoc != nullptr)
            debugString.append("\n" + jsonDoc->toJson(QJsonDocument::JsonFormat::Indented));

        emit showDebugSignal(QTime::currentTime(), debugString);
        m_settings->debugOut(VN_S(VMTDNxApiAdapter) + QString("(%1) | %2")
                             .arg(m_url.toString(QUrl::RemoveUserInfo))
                             .arg(debugString));
    }
}

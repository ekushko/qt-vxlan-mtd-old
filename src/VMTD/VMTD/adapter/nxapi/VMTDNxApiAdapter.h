#pragma once

#include "../../VMTDSettings.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QPointer>
#include <QTimer>

namespace VMTDLib
{
    class VMTDNxApiAdapterForm;

    class VMTD_SHARED VMTDNxApiAdapter : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(VMTDNxApiAdapter)

    public:

        VMTDNxApiAdapter(QObject *parent,
                         VMTDSettings *settings,
                         QNetworkAccessManager *netManager);
        ~VMTDNxApiAdapter();

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        VMTDSettings *settings() const;

        QUrl    url() const;
        void setUrl(const QUrl &url);

    signals:

        void showDebugSignal(const QTime &time, const QString &text);

        void commandExecutedSignal(bool ok);

    public slots:

        void showFormSlot();

        void checkConnectionSlot();

        void sendCommandSlot(const QStringList &commands);

        void replyFinishedSlot(QNetworkReply *reply);

    private:

        void buildRequest(QNetworkRequest *request);
        void  showRequest(QNetworkRequest *request, QJsonDocument *jsonDoc = nullptr);

        VMTDSettings *m_settings;

        QNetworkAccessManager *m_netManager;

        QPointer<VMTDNxApiAdapterForm> m_form;

        QUrl m_url;

        int m_commandCounter = 1;
    };
}

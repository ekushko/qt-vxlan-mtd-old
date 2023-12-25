#pragma once

#include "../VMTDSettings.h"

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

        void showForm();

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        VMTDSettings *settings() const;

        bool isConnected() const;

        bool canSend() const;

        QUrl    url() const;
        void setUrl(const QUrl &url);

        int     ticketTimeoutInterval() const;
        void setTicketTimeoutInterval(int ticketTimeoutInterval);

    signals:

        void showMessageSignal(const QString &text);

    public slots:

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

        bool m_isConnected;
        QTimer m_ticketTimeout;
        int m_ticketTimeoutInterval;

    private slots:

        void ticketTimeoutSlot();
    };
}

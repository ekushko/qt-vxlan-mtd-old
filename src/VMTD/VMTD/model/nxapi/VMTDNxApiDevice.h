#pragma once

#include "../../VMTDSettings.h"
#include "../interface/VMTDInterfaces.h"

#include <QUrl>

namespace VMTDLib
{
    class VMTDNxApiDeviceForm;

    class VMTD_SHARED VMTDNxApiDevice : public QObject
    {
        Q_OBJECT

    public:

        VMTDNxApiDevice(QObject *parent, VMTDSettings *settings);
        ~VMTDNxApiDevice();

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        // МЕТАДАННЫЕ

        bool  isOnline() const;
        void setOnline(bool isOnline);

        // ДАННЫЕ

        int     id() const;
        void setId(int id);

        QUrl    url() const;
        void setUrl(const QUrl &url);

        int     ticketTimeoutInterval() const;
        void setTicketTimeoutInterval(int ticketTimeoutInterval);

        VMTDInterfaces *interfaces() const;

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDNxApiDeviceForm> m_form;

        VMTDSettings *m_settings;
        VMTDInterfaces *m_interfaces;

        bool m_isOnline = false;

        int m_id = -1;
        QUrl m_url = QUrl("http://127.0.0.1/ins");
        int m_ticketTimeoutInterval = 1000;
    };
}

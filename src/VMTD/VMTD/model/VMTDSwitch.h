#pragma once

#include "../VMTDSettings.h"

#include <QUrl>
#include <QMap>

namespace VMTDLib
{
    class VMTD_SHARED VMTDSwitch : public QObject
    {
        Q_OBJECT

    public:

        VMTDSwitch(QObject *parent, VMTDSettings *settings);

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

        int     portCount() const;
        void setPortCount(int portCount);

        QVector<int> PortToNode;

        QVector<QString> PortToInterface;

    signals:

        void portCountChangedSignal();

    private:

        VMTDSettings *m_settings;

        bool m_isOnline = false;

        int m_id = -1;
        QUrl m_url = QUrl("http://127.0.0.1/ins");
        int m_ticketTimeoutInterval = 1000;
        int m_portCount = 10;
    };
}

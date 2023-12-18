#pragma once

#include "VMTDDeclareShared.h"

#include <QObject>
#include <QJsonObject>
#include <QPointer>

namespace VMTDLib
{
    class VMTDSettingsForm;

    class VMTDSettings : public QObject
    {
        Q_OBJECT

    public:

        VMTDSettings(QObject *parent, const QString &systemName);
        ~VMTDSettings();

        void showForm();

        void debugOut(const QString &text);

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        QString filePath() const;

        void save();
        void load();

        void apply();

        // СИСТЕМНЫЕ

        QString systemName() const;

        QString debugName() const;
        void setDebugName(const QString &debugName);

        bool    shouldShowDebug() const;
        void setShouldShowDebug(bool shouldShowDebug);

        // ПАРАМЕТРЫ СЕРВЕРА

        int     localPort() const;
        void setLocalPort(int localPort);

        // ПАРАМЕТРЫ КЛИЕНТА

        QString serverIp() const;
        void setServerIp(const QString &serverIp);

        int     serverPort() const;
        void setServerPort(int serverPort);

        bool    shouldReconnect() const;
        void setShouldReconnect(bool shouldReconnect);

        int     reconnectInterval() const;
        void setReconnectInterval(int reconnectInterval);

        // ПАРАМЕТРЫ АДАПТЕРОВ NX-API

        QJsonObject nxApiAdaptersParams() const;
        void     setNxApiAdaptersParams(const QJsonObject &nxApiAdaptersParams);

    signals:

        void networkChangedSignal();

        void saveSignal();
        void loadSignal();

    private:

        const QString DIR_NAME = "settings";
        const QString FILE_EXTENSION = ".settings";

        QPointer<VMTDSettingsForm> m_form;

        const QString m_systemName;
        QString m_debugName;
        bool m_shouldShowDebug;

        QString m_serverIp;
        int     m_serverPort;
        bool m_shouldReconnect;
        int m_reconnectInterval;

        int m_localPort;

        QJsonObject m_nxApiAdatersParams;

        bool m_wasNetworkChanged;

    private slots:

        void saveSlot();
        void loadSlot();
    };
}

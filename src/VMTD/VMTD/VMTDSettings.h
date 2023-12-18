#pragma once

#include "VMTDDeclareShared.h"

#include <QObject>
#include <QJsonObject>
#include <QPointer>

namespace VMTDLib
{
#define VMTDNodeType VMTDSettings::EnNodeType

    class VMTDSettingsForm;

    class VMTDSettings : public QObject
    {
        Q_OBJECT

    public:

        enum class EnNodeType;

        VMTDSettings(QObject *parent, EnNodeType nodeType, const QString &systemName);
        ~VMTDSettings();

        void showForm();

        // КОНСТАНТЫ

        enum class EnNodeType
        {
            CLIENT = 0,
            SERVER
        };
        Q_ENUM(EnNodeType)
        static const QString            &enNodeTypeToS(const EnNodeType &nodeType);
        static const QMap<int, QString> &enNodeTypeToL();

        // ЛОГИКА

        void debugOut(const QString &text);

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        QString filePath() const;

        void save();
        void load();

        void apply();

        // СИСТЕМНЫЕ

        EnNodeType nodeType() const;

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

        const EnNodeType m_nodeType;
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

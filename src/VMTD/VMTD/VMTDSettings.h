#pragma once

#include "VMTDDeclareShared.h"

#include <QObject>
#include <QJsonObject>
#include <QPointer>
#include <QMutex>

namespace VMTDLib
{
#define VMTDNodeType VMTDSettings::EnNodeType

    class VMTDSettingsForm;

    class VMTD_SHARED VMTDSettings : public QObject
    {
        Q_OBJECT

    public:

        VMTDSettings(QObject *parent, const QString &systemName);
        ~VMTDSettings();

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

        int generateId();

        void debugOut(const QString &text);

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        QString filePath() const;

        void save();
        void load();

        void apply();

        // СИСТЕМНЫЕ

        EnNodeType nodeType() const;
        void    setNodeType(const EnNodeType &nodeType);

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

        // ПАРАМЕТРЫ ПРОТОКОЛА

        bool    shouldCheckConnection() const;
        void setShouldCheckConnection(bool shouldCheckConnection);

        int     checkConnectionInterval() const;
        void setCheckConnectionInterval(int checkConnectionInterval);

        int     reconnectInterval() const;
        void setReconnectInterval(int reconnectInterval);

        int     ticketTimeoutInterval() const;
        void setTicketTimeoutInterval(int ticketTimeoutInterval);

        int     checkQueueInterval() const;
        void setCheckQueueInterval(int checkQueueInterval);

        // УСТРОЙСТВА

        QJsonObject deviceManagerObj() const;
        void     setDeviceManagerObj(const QJsonObject &deviceManagerObj);

        // ПОДКЛЮЧЕНИЯ

        QJsonObject connectionManagerObj() const;
        void     setConnectionManagerObj(const QJsonObject &connectionManagerObj);

    signals:

        void restartSignal();

        void saveSignal();
        void loadSignal();

    public slots:

        void showFormSlot();

    private:

        const QString DIR_NAME = "settings";
        const QString FILE_EXTENSION = ".settings";

        QPointer<VMTDSettingsForm> m_form;

        EnNodeType m_nodeType;
        const QString m_systemName;
        QString m_debugName;
        bool m_shouldShowDebug;

        QString m_serverIp;
        int     m_serverPort;
        bool m_shouldReconnect;
        int m_reconnectInterval;

        int m_localPort;
        bool m_shouldCheckConnection;
        int m_checkConnectionInterval;

        int m_ticketTimeoutInterval;
        int m_checkQueueInterval;

        QJsonObject m_deviceManagerObj;
        QJsonObject m_connectionManagerObj;

        int m_idCounter;

        bool m_shouldBeRestarted;

        QMutex m_locker;

    private slots:

        void saveSlot();
        void loadSlot();
    };
}

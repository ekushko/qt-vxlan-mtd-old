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

        QString systemName() const;

        QString debugName() const;
        void setDebugName(const QString &debugName);

        bool    shouldShowDebug() const;
        void setShouldShowDebug(bool shouldShowDebug);

        QString serverIp() const;
        void setServerIp(const QString &serverIp);

        int     serverPort() const;
        void setServerPort(int serverPort);

        int     localPort() const;
        void setLocalPort(int localPort);

        QJsonObject nxApiAdaptersParams() const;
        void     setNxApiAdaptersParams(const QJsonObject &nxApiAdaptersParams);

    signals:
        void networkChangedSignal();

        void saveSignal();
        void loadSignal();

    private slots:
        void saveSlot();
        void loadSlot();

    private:
        const QString DIR_NAME = "settings";
        const QString FILE_EXTENSION = ".settings";

        QPointer<VMTDSettingsForm> m_form;

        const QString m_systemName;
        QString m_debugName;
        bool m_shouldShowDebug;

        QString m_serverIp;
        int     m_serverPort;

        int m_localPort;

        bool m_wasNetworkChanged;

        QJsonObject m_nxApiAdatersParams;
    };
}

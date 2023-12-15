#include "VMTDSettings.h"
#include "VMTDSettingsForm.h"

#include "VMTDRepo.h"

#include <QDir>
#include <QJsonDocument>
#include <QDebug>

namespace VMTDLib
{
    VMTDSettings::VMTDSettings(QObject *parent, const QString &systemName)
        : QObject{parent}
        , m_systemName{systemName}
    {
        m_debugName = "VMTD";
        m_shouldShowDebug = true;
        m_serverIp = "127.0.0.1";
        m_serverPort = 30000;
        m_shouldReconnect = false;
        m_reconnectInterval = 1000;
        m_localPort = 30001;

        m_wasNetworkChanged = false;

        debugOut(VN_S(VMTDSettings) + " was created");

        connect(this, &VMTDSettings::saveSignal, this, &VMTDSettings::saveSlot);
        connect(this, &VMTDSettings::loadSignal, this, &VMTDSettings::loadSlot);

        if (!QDir(DIR_NAME).exists())
        {
            if (!QDir().mkdir(DIR_NAME))
            {
                debugOut(VN_S(VMTDSettings) + " | Dir creating: could not create directory " + DIR_NAME);
                return;
            }
        }

        load();
    }

    VMTDSettings::~VMTDSettings()
    {
        debugOut(VN_S(VMTDSettings) + " was deleted");

        if (m_form != nullptr)
            delete m_form;
    }

    void VMTDSettings::showForm()
    {
        if (m_form == nullptr)
            m_form = new VMTDSettingsForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDSettings::debugOut(const QString &text)
    {
        if (m_shouldShowDebug)
            qDebug() << m_debugName +  " | " + text;
    }

    QJsonObject VMTDSettings::toJson() const
    {
        QJsonObject jsonObj;

        jsonObj[VN_ME(m_systemName)] = m_systemName;
        jsonObj[VN_ME(m_debugName)] = m_debugName;
        jsonObj[VN_ME(m_shouldShowDebug)] = m_shouldShowDebug;
        jsonObj[VN_ME(m_serverIp)] = m_serverIp;
        jsonObj[VN_ME(m_serverPort)] = m_serverPort;
        jsonObj[VN_ME(m_localPort)] = m_localPort;

        jsonObj[VN_ME(m_nxApiAdaptersParams)] = m_nxApiAdatersParams;

        return jsonObj;
    }
    void VMTDSettings::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        m_debugName = jsonObj[VN_ME(m_debugName)].toString(m_debugName);
        m_shouldShowDebug = jsonObj[VN_ME(m_shouldShowDebug)].toBool(m_shouldShowDebug);
        m_serverIp = jsonObj[VN_ME(m_serverIp)].toString(m_serverIp);
        m_serverPort = jsonObj[VN_ME(m_serverPort)].toInt(m_serverPort);
        m_localPort = jsonObj[VN_ME(m_localPort)].toInt(m_localPort);

        m_nxApiAdatersParams = jsonObj[VN_ME(m_nxApiAdaptersParams)].toObject();
    }

    QString VMTDSettings::filePath() const
    {
        QString fileName = QString("%1%2%3%4")
                           .arg(DIR_NAME,
                                QDir::separator(),
                                m_systemName,
                                FILE_EXTENSION);

        return fileName;
    }

    void VMTDSettings::save()
    {
        emit saveSignal();
    }
    void VMTDSettings::load()
    {
        emit loadSignal();
    }

    void VMTDSettings::apply()
    {
        if (m_wasNetworkChanged)
        {
            emit networkChangedSignal();

            m_wasNetworkChanged = false;
        }
    }

    QString VMTDSettings::systemName() const
    {
        return m_systemName;
    }

    QString VMTDSettings::debugName() const
    {
        return m_debugName;
    }
    void VMTDSettings::setDebugName(const QString &debugName)
    {
        m_debugName = debugName;
    }

    bool VMTDSettings::shouldShowDebug() const
    {
        return m_shouldShowDebug;
    }
    void VMTDSettings::setShouldShowDebug(bool shouldShowDebug)
    {
        m_shouldShowDebug = shouldShowDebug;
    }

    QString VMTDSettings::serverIp() const
    {
        return m_serverIp;
    }
    void VMTDSettings::setServerIp(const QString &serverIp)
    {
        if (m_serverIp != serverIp)
        {
            m_serverIp = serverIp;

            m_wasNetworkChanged = true;
        }
    }

    int VMTDSettings::serverPort() const
    {
        return m_serverPort;
    }
    void VMTDSettings::setServerPort(int serverPort)
    {
        if (m_serverPort != serverPort)
        {
            m_serverPort = serverPort;

            m_wasNetworkChanged = true;
        }
    }

    int VMTDSettings::localPort() const
    {
        return m_localPort;
    }
    void VMTDSettings::setLocalPort(int localPort)
    {
        if (m_localPort != localPort)
        {
            m_localPort = localPort;

            m_wasNetworkChanged = true;
        }
    }

    bool VMTDSettings::shouldReconnect() const
    {
        return m_shouldReconnect;
    }
    void VMTDSettings::setShouldReconnect(bool shouldReconnect)
    {
        if (m_shouldReconnect != shouldReconnect)
        {
            m_shouldReconnect = shouldReconnect;

            m_wasNetworkChanged = true;
        }
    }

    int VMTDSettings::reconnectInterval() const
    {
        return m_reconnectInterval;
    }
    void VMTDSettings::setReconnectInterval(int reconnectInterval)
    {
        if (m_reconnectInterval != reconnectInterval)
        {
            m_reconnectInterval = reconnectInterval;

            m_wasNetworkChanged = true;
        }
    }

    QJsonObject VMTDSettings::nxApiAdaptersParams() const
    {
        return m_nxApiAdatersParams;
    }
    void VMTDSettings::setNxApiAdaptersParams(const QJsonObject &nxApiAdaptersParams)
    {
        m_nxApiAdatersParams = nxApiAdaptersParams;
    }

    void VMTDSettings::saveSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.open(QIODevice::WriteOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Save: could not open settings file");
        }
        else
        {
            QJsonDocument jsonDoc;
            jsonDoc.setObject(toJson());
            settingsFile.write(jsonDoc.toJson());
            settingsFile.close();

            debugOut(VN_S(VMTDSettings) + " | Settings was saved successfully.");
        }
    }
    void VMTDSettings::loadSlot()
    {
        QFile settingsFile(filePath());

        if (!settingsFile.exists())
        {
            saveSlot();
            return;
        }

        if (!settingsFile.open(QIODevice::ReadOnly))
        {
            debugOut(VN_S(VMTDSettings) + " | Load: could not open settings file");
        }
        else
        {
            QByteArray settingsData = settingsFile.readAll();
            settingsFile.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(settingsData);
            fromJson(jsonDoc.object());

            debugOut(VN_S(VMTDSettings) + " | Settings was loaded successfully.");
        }
    }
}

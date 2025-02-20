#include "VMTDSettings.h"
#include "VMTDSettingsForm.h"

#include "VMTDRepo.h"

#include <QDir>
#include <QJsonDocument>
#include <QDebug>

namespace VMTDLib
{
    VMTDSettings::VMTDSettings(QObject *parent, const QString &systemName)
        : QObject(parent)
        , m_systemName(systemName)
    {
        // системные параметры

        m_mainWidget = nullptr;
        m_nodeType = EnNodeType::CLIENT;
        m_debugName = "VMTD";
        m_shouldShowDebug = true;
        m_shouldShowDebugCreation = false;
        m_shouldShowFullScreen = true;
        m_isTrayMode = false;

        // параметры сервера

        m_localPort = 30001;

        // параметры клиента

        m_serverIp = "127.0.0.1";
        m_serverPort = 30000;
        m_shouldReconnect = false;
        m_reconnectInterval = 1000;

        // параметры протокола

        m_shouldCheckConnection = false;
        m_checkConnectionInterval = 100;
        m_checkQueueInterval = 100;
        m_ticketTimeoutInterval = 3000;

        // параметры движителя

        m_shouldCheckOnline = false;
        m_netplan1FilePath = QString("%1etc%1netplan%102-vmtd-1.yaml")
                             .arg(QDir::separator());
        m_netplan2FilePath = QString("%1etc%1netplan%103-vmtd-2.yaml")
                             .arg(QDir::separator());
        m_minVlanId = 20;
        m_maxVlanId = 50;

        // прочее

        m_idCounter = 0;
        m_shouldBeRestarted = false;

        creationOut(VN_S(VMTDSettings) + " | Constructor called");

        connect(this, &VMTDSettings::saveSignal, this, &VMTDSettings::saveSlot);
        connect(this, &VMTDSettings::loadSignal, this, &VMTDSettings::loadSlot);

        if (!QDir(DIR_NAME).exists())
        {
            if (!QDir().mkdir(DIR_NAME))
            {
                debugOut(VN_S(VMTDSettings) + " | DIR creating: could not create directory " + DIR_NAME);
                return;
            }
        }

        load();

        creationOut(VN_S(VMTDSettings) + " | Constructor finished");
    }

    VMTDSettings::~VMTDSettings()
    {
        creationOut(VN_S(VMTDSettings) + " | Destructor called");

        if (m_form != nullptr)
            delete m_form;

        creationOut(VN_S(VMTDSettings) + " | Destructor finished");
    }

    const QString &VMTDSettings::enNodeTypeToS(const EnNodeType &nodeType)
    {
        switch (nodeType)
        {
        // *INDENT-OFF*
        case EnNodeType::CLIENT : RETURN_S("Client");
        case EnNodeType::SERVER : RETURN_S("Server");
        // *INDENT-ON*
        }

        return S_QUESTIONS;
    }
    const QMap<int, QString> &VMTDSettings::enNodeTypeToL()
    {
        RETURN_MAP(EnNodeType, enNodeTypeToS);
    }

    int VMTDSettings::generateId()
    {
        QMutexLocker lock(&m_locker);

        return ++m_idCounter;
    }

    void VMTDSettings::debugOut(const QString &text)
    {
        if (m_shouldShowDebug)
            qDebug() << m_debugName +  " | " + text;
    }
    void VMTDSettings::creationOut(const QString &text)
    {
        if (m_shouldShowDebugCreation)
            qDebug() << m_debugName +  " | " + text;
    }

    QJsonObject VMTDSettings::toJson() const
    {
        QJsonObject jsonObj;

        // системные параметры

        jsonObj[VN_ME(m_nodeType)] = static_cast<int>(m_nodeType);
        jsonObj[VN_ME(m_systemName)] = m_systemName;
        jsonObj[VN_ME(m_debugName)] = m_debugName;
        jsonObj[VN_ME(m_shouldShowDebug)] = m_shouldShowDebug;
        jsonObj[VN_ME(m_shouldShowDebugCreation)] = m_shouldShowDebugCreation;
        jsonObj[VN_ME(m_shouldShowFullScreen)] = m_shouldShowFullScreen;

        // параметры сервера

        jsonObj[VN_ME(m_localPort)] = m_localPort;

        // параметры клиента

        jsonObj[VN_ME(m_serverIp)] = m_serverIp;
        jsonObj[VN_ME(m_serverPort)] = m_serverPort;
        jsonObj[VN_ME(m_shouldReconnect)] = m_shouldReconnect;
        jsonObj[VN_ME(m_reconnectInterval)] = m_reconnectInterval;

        // параметры протокола

        jsonObj[VN_ME(m_shouldCheckConnection)] = m_shouldCheckConnection;
        jsonObj[VN_ME(m_checkConnectionInterval)] = m_checkConnectionInterval;
        jsonObj[VN_ME(m_checkQueueInterval)] = m_checkQueueInterval;
        jsonObj[VN_ME(m_ticketTimeoutInterval)] = m_ticketTimeoutInterval;

        // параметры движителя

        jsonObj[VN_ME(m_shouldCheckOnline)] = m_shouldCheckOnline;
        jsonObj[VN_ME(m_netplan1FilePath)] = m_netplan1FilePath;
        jsonObj[VN_ME(m_netplan2FilePath)] = m_netplan2FilePath;
        jsonObj[VN_ME(m_minVlanId)] = m_minVlanId;
        jsonObj[VN_ME(m_maxVlanId)] = m_maxVlanId;

        // устройства

        jsonObj[VN_ME(m_deviceManagerObj)] = m_deviceManagerObj;

        // подключения

        jsonObj[VN_ME(m_connectionManagerObj)] = m_connectionManagerObj;

        // прочее

        jsonObj[VN_ME(m_idCounter)] = m_idCounter;

        return jsonObj;
    }
    void VMTDSettings::fromJson(const QJsonObject &jsonObj)
    {
        if (jsonObj.isEmpty())
            return;

        // системные параметры

        m_nodeType = (EnNodeType)jsonObj[VN_ME(m_nodeType)].toInt((int)m_nodeType);
        m_debugName = jsonObj[VN_ME(m_debugName)].toString(m_debugName);
        m_shouldShowDebug = jsonObj[VN_ME(m_shouldShowDebug)].toBool(m_shouldShowDebug);
        m_shouldShowDebugCreation = jsonObj[VN_ME(m_shouldShowDebugCreation)]
                                    .toBool(m_shouldShowDebugCreation);
        m_shouldShowFullScreen = jsonObj[VN_ME(m_shouldShowFullScreen)]
                                 .toBool(m_shouldShowFullScreen);

        // параметры сервера

        m_localPort = jsonObj[VN_ME(m_localPort)].toInt(m_localPort);

        // параметры клиента

        m_serverIp = jsonObj[VN_ME(m_serverIp)].toString(m_serverIp);
        m_serverPort = jsonObj[VN_ME(m_serverPort)].toInt(m_serverPort);
        m_shouldReconnect = jsonObj[VN_ME(m_shouldReconnect)].toBool(m_shouldReconnect);
        m_reconnectInterval = jsonObj[VN_ME(m_reconnectInterval)].toInt(m_reconnectInterval);

        // параметры протокола

        m_shouldCheckConnection = jsonObj[VN_ME(m_shouldCheckConnection)]
                                  .toBool(m_shouldCheckConnection);
        m_checkConnectionInterval = jsonObj[VN_ME(m_checkConnectionInterval)]
                                    .toInt(m_checkConnectionInterval);
        m_checkQueueInterval = jsonObj[VN_ME(m_checkQueueInterval)]
                               .toInt(m_checkQueueInterval);
        m_ticketTimeoutInterval = jsonObj[VN_ME(m_ticketTimeoutInterval)]
                                  .toInt(m_ticketTimeoutInterval);

        // параметры движителя

        m_shouldCheckOnline = jsonObj[VN_ME(m_shouldCheckOnline)].toBool(m_shouldCheckOnline);
        m_netplan1FilePath = jsonObj[VN_ME(m_netplan1FilePath)].toString(m_netplan1FilePath);
        m_netplan2FilePath = jsonObj[VN_ME(m_netplan2FilePath)].toString(m_netplan2FilePath);
        m_minVlanId = jsonObj[VN_ME(m_minVlanId)].toInt(m_minVlanId);
        m_maxVlanId = jsonObj[VN_ME(m_maxVlanId)].toInt(m_maxVlanId);

        // устройства

        m_deviceManagerObj = jsonObj[VN_ME(m_deviceManagerObj)].toObject();

        // подключения

        m_connectionManagerObj = jsonObj[VN_ME(m_connectionManagerObj)].toObject();

        // прочее

        m_idCounter = jsonObj[VN_ME(m_idCounter)].toInt(m_idCounter);
    }

    QString VMTDSettings::filePath() const
    {
        QString filePath = QString("%1%2%3%4")
                           .arg(DIR_NAME,
                                QDir::separator(),
                                m_systemName,
                                FILE_EXTENSION);

        return filePath;
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
        if (m_shouldBeRestarted)
        {
            emit restartSignal();

            m_shouldBeRestarted = false;
        }
    }

    QStackedWidget *VMTDSettings::mainWidget() const
    {
        return m_mainWidget;
    }
    void VMTDSettings::setMainWidget(QStackedWidget *mainWidget)
    {
        m_mainWidget = mainWidget;
    }

    VMTDSettings::EnNodeType VMTDSettings::nodeType() const
    {
        return m_nodeType;
    }
    void VMTDSettings::setNodeType(const VMTDSettings::EnNodeType &nodeType)
    {
        if (m_nodeType != nodeType)
        {
            m_nodeType = nodeType;

            m_shouldBeRestarted = true;
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

    bool VMTDSettings::shouldShowDebugCreation() const
    {
        return m_shouldShowDebugCreation;
    }
    void VMTDSettings::setShouldShowDebugCreation(bool shouldShowDebugCreation)
    {
        m_shouldShowDebugCreation = shouldShowDebugCreation;
    }

    bool VMTDSettings::shouldShowFullScreen() const
    {
        return m_shouldShowFullScreen;
    }
    void VMTDSettings::setShouldShowFullScreen(bool shouldShowFullScreen)
    {
        m_shouldShowFullScreen = shouldShowFullScreen;
    }

    bool VMTDSettings::isTrayMode() const
    {
        return m_isTrayMode;
    }
    void VMTDSettings::setTrayMode(bool isTrayMode)
    {
        m_isTrayMode = isTrayMode;
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

            m_shouldBeRestarted = true;
        }
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

            m_shouldBeRestarted = true;
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

            m_shouldBeRestarted = true;
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

            m_shouldBeRestarted = true;
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

            m_shouldBeRestarted = true;
        }
    }


    bool VMTDSettings::shouldCheckConnection() const
    {
        return m_shouldCheckConnection;
    }
    void VMTDSettings::setShouldCheckConnection(bool shouldCheckConnection)
    {
        m_shouldCheckConnection = shouldCheckConnection;
    }

    int VMTDSettings::checkConnectionInterval() const
    {
        return m_checkConnectionInterval;
    }
    void VMTDSettings::setCheckConnectionInterval(int checkConnectionInterval)
    {
        if (m_checkConnectionInterval != checkConnectionInterval)
        {
            m_checkConnectionInterval = checkConnectionInterval;

            m_shouldBeRestarted = true;
        }
    }

    int VMTDSettings::ticketTimeoutInterval() const
    {
        return m_ticketTimeoutInterval;
    }
    void VMTDSettings::setTicketTimeoutInterval(int ticketTimeoutInterval)
    {
        m_ticketTimeoutInterval = ticketTimeoutInterval;
    }

    int VMTDSettings::checkQueueInterval() const
    {
        return m_checkQueueInterval;
    }
    void VMTDSettings::setCheckQueueInterval(int checkQueueInterval)
    {
        if (m_checkQueueInterval != checkQueueInterval)
        {
            m_checkQueueInterval = checkQueueInterval;

            m_shouldBeRestarted = true;
        }
    }

    bool VMTDSettings::shouldCheckOnline() const
    {
        return m_shouldCheckOnline;
    }
    void VMTDSettings::setShouldCheckOnline(bool shouldCheckOnline)
    {
        m_shouldCheckOnline = shouldCheckOnline;
    }

    QString VMTDSettings::netplan1FilePath() const
    {
        return m_netplan1FilePath;
    }
    void VMTDSettings::setNetplan1FilePath(const QString &netplan1FilePath)
    {
        m_netplan1FilePath = netplan1FilePath;
    }

    QString VMTDSettings::netplan2FilePath() const
    {
        return m_netplan2FilePath;
    }
    void VMTDSettings::setNetplan2FilePath(const QString &netplan2FilePath)
    {
        m_netplan2FilePath = netplan2FilePath;
    }

    int VMTDSettings::minVlanId() const
    {
        return m_minVlanId;
    }
    void VMTDSettings::setMinVlanId(int minVlanId)
    {
        m_minVlanId = minVlanId;
    }

    int VMTDSettings::maxVlanId() const
    {
        return m_maxVlanId;
    }
    void VMTDSettings::setMaxVlanId(int maxVlanId)
    {
        m_maxVlanId = maxVlanId;
    }


    QJsonObject VMTDSettings::deviceManagerObj() const
    {
        return m_deviceManagerObj;
    }
    void VMTDSettings::setDeviceManagerObj(const QJsonObject &deviceManagerObj)
    {
        m_deviceManagerObj = deviceManagerObj;
    }

    QJsonObject VMTDSettings::connectionManagerObj() const
    {
        return m_connectionManagerObj;
    }
    void VMTDSettings::setConnectionManagerObj(const QJsonObject &connectionManagerObj)
    {
        m_connectionManagerObj = connectionManagerObj;
    }


    void VMTDSettings::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDSettingsForm(m_mainWidget, this);

        if (m_mainWidget != nullptr)
        {
            m_mainWidget->addWidget(m_form);
            m_mainWidget->setCurrentWidget(m_form);
        }

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
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

            debugOut(VN_S(VMTDSettings) + " | Settings were saved successfully.");
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

            debugOut(VN_S(VMTDSettings) + " | Settings were loaded successfully.");
        }
    }
}

#include "VMTDNxApiServer.h"
#include "VMTDNxApiServerForm.h"

#include "../../VMTDRepo.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDNxApiServer::VMTDNxApiServer(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Constructor called");

        m_netManager = new QNetworkAccessManager(this);

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Constructor finished");
    }

    VMTDNxApiServer::~VMTDNxApiServer()
    {
        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Destructor called");

        if (m_form != nullptr)
            m_form->deleteLater();

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Destructor finished");
    }

    VMTDSettings *VMTDNxApiServer::settings() const
    {
        return m_settings;
    }

    bool VMTDNxApiServer::isListening() const
    {
        return m_isListening;
    }

    const QVector<VMTDNxApiAdapter *> &VMTDNxApiServer::adapters() const
    {
        return m_adapters;
    }

    void VMTDNxApiServer::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiServerForm(parent, this);
        else
            m_form->setParent(parent);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNxApiServer::startListenSlot()
    {
        if (m_isListening)
        {
            m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Server already listen!");
            return;
        }

        const auto modelObj = m_settings->deviceManagerObj();

        const auto nxApiDevicesArr = modelObj["nxApiDevices"].toArray();

        for (int i = 0; i < nxApiDevicesArr.size(); ++i)
        {
            auto adapter = new VMTDNxApiAdapter(this, m_settings, m_netManager);
            adapter->fromJson(nxApiDevicesArr.at(i).toObject());

            // *INDENT-OFF*
            const auto res = std::find_if(m_adapters.begin(), m_adapters.end(),
                                          [adapter](VMTDNxApiAdapter *createdAdapter)
            {
                return createdAdapter->url().toString(QUrl::RemoveUserInfo)
                             == adapter->url().toString(QUrl::RemoveUserInfo);
            });
            // *INDENT-ON*

            if (res == m_adapters.end())
            {
                m_adapters.append(adapter);
                emit adapterCreatedSignal(adapter);
            }
            else
            {
                delete adapter;
            }
        }

        m_isListening = true;

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Start listening...");
    }
    void VMTDNxApiServer::stopListenSlot()
    {
        if (!m_isListening)
        {
            m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Server is not listening!");
            return;
        }

        for (auto adapter : m_adapters)
            emit adapterRemovedSignal(adapter);

        qDeleteAll(m_adapters);
        m_adapters.clear();

        m_isListening = false;

        m_settings->debugOut(VN_S(VMTDNxApiServer) + " | Listening finished!");
    }
    void VMTDNxApiServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }
}

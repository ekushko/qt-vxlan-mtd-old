#include "VMTDNxApiServer.h"
#include "VMTDNxApiServerForm.h"

#include <QJsonArray>

#include <algorithm>

namespace VMTDLib
{
    VMTDNxApiServer::VMTDNxApiServer(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {
        m_netManager = new QNetworkAccessManager(this);
    }

    VMTDNxApiServer::~VMTDNxApiServer()
    {
        if (m_form != nullptr)
            m_form->deleteLater();
    }

    bool VMTDNxApiServer::isListening() const
    {
        return m_isListening;
    }

    const QVector<VMTDNxApiAdapter *> &VMTDNxApiServer::adapters() const
    {
        return m_adapters;
    }

    void VMTDNxApiServer::showFormSlot()
    {
        if (m_form == nullptr)
            m_form = new VMTDNxApiServerForm(nullptr, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDNxApiServer::startListenSlot()
    {
        if (m_isListening)
            return;

        const auto modelObj = m_settings->modelObj();

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
    }
    void VMTDNxApiServer::stopListenSlot()
    {
        if (!m_isListening)
            return;

        for (auto adapter : m_adapters)
            emit adapterRemovedSignal(adapter);

        qDeleteAll(m_adapters);
        m_adapters.clear();

        m_isListening = false;
    }
    void VMTDNxApiServer::restartListenSlot()
    {
        stopListenSlot();
        startListenSlot();
    }
}

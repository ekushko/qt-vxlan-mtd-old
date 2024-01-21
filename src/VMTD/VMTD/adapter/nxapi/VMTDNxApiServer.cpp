#include "VMTDNxApiServer.h"
#include "VMTDNxApiServerForm.h"

#include <QJsonArray>

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

        auto modelObj = m_settings->modelObj();

        for (int i = 0; i < modelObj.size(); ++i)
        {
            auto adapter = new VMTDNxApiAdapter(this, m_settings, m_netManager);
            m_adapters.append(adapter);

            const auto switchesArr = modelObj["switches"].toArray();

            for (int i = 0; i < switchesArr.size(); ++i)
                adapter->fromJson(switchesArr.at(i).toObject());
        }

        m_isListening = true;
    }
    void VMTDNxApiServer::stopListenSlot()
    {
        if (!m_isListening)
            return;

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

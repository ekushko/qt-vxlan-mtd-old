#pragma once

#include "VMTDNxApiAdapter.h"

#include <QNetworkAccessManager>

namespace VMTDLib
{
    class VMTDNxApiServerForm;

    class VMTDNxApiServer : public QObject
    {
        Q_OBJECT

    public:

        VMTDNxApiServer(QObject *parent, VMTDSettings *settings);
        ~VMTDNxApiServer();

        bool isListening() const;

        QString stateString() const;

        const QVector<VMTDNxApiAdapter *> &adapters() const;

    signals:

        void adapterCreatedSignal(VMTDNxApiAdapter *adapter);

    public slots:

        void showFormSlot();

        void   startListenSlot();
        void    stopListenSlot();
        void restartListenSlot();

    private:

        QPointer<VMTDNxApiServerForm> m_form;

        VMTDSettings *m_settings;

        QNetworkAccessManager *m_netManager;

        QVector<VMTDNxApiAdapter *> m_adapters;

        bool m_isListening = false;
    };
}

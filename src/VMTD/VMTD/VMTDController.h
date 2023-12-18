#pragma once

#include "VMTDSettings.h"

#include "adapters/VMTDNxApiAdapter.h"

#include <QThread>
#include <QNetworkAccessManager>

namespace VMTDLib
{
    class VMTDControllerForm;

    class VMTDController : public QThread
    {
        Q_OBJECT

    public:

        VMTDController(QObject *parent, VMTDSettings *settings);
        ~VMTDController();

        void showForm();

        VMTDSettings *settings() const;

        void startController();
        void  stopController();

        const QVector<VMTDNxApiAdapter *> &nxApiAdapters() const;
        void saveNxApiAdapters();
        void loadNxApiAdapters();

        VMTDNxApiAdapter *createNxApiAdapter();
        void              deleteNxApiAdapter(int index);

    protected:

        void run() override;

    private:

        QPointer<VMTDControllerForm> m_form;

        VMTDSettings *m_settings;

        QNetworkAccessManager *m_netManager;

        QVector<VMTDNxApiAdapter *> m_nxApiAdapters;

    private slots:

        void  startedSlot();
        void finishedSlot();
    };
}

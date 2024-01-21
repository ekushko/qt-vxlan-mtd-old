#pragma once

#include "VMTDNxApiServer.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDNxApiServerForm;
}

namespace VMTDLib
{
    class VMTDNxApiServerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiServerForm(QWidget *parent, VMTDNxApiServer *server);
        ~VMTDNxApiServerForm();

    private:

        Ui::VMTDNxApiServerForm *ui;

        VMTDNxApiServer *m_server;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void updateAdaptersListSlot();

        void lwAdaptersDoubleClicked(QListWidgetItem *item);
    };
}

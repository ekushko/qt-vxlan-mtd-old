#pragma once

#include "VMTDNodeClient.h"

#include "VMTDNodeAdapterForm.h"

namespace Ui
{
    class VMTDNodeClientForm;
}

namespace VMTDLib
{
    class VMTDNodeClientForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNodeClientForm(QWidget *parent, VMTDNodeClient *client);
        ~VMTDNodeClientForm();

    private:

        void initializeView();
        void updateView();

        Ui::VMTDNodeClientForm *ui;

        VMTDNodeClient *m_client;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

        VMTDNodeAdapterForm *m_adapterForm;

    private slots:

        void uiTimerTickSlot();

        void pbShowDetailedStateClicked();
    };
}

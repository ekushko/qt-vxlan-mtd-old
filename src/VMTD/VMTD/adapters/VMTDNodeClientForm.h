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

    signals:

        void sendRequestSignal(QWebSocket *socket, const QJsonObject &requestObj);

    private:

        void initializeView();
        void updateView();

        Ui::VMTDNodeClientForm *ui;

        VMTDNodeClient *m_client;

        QTimer m_uiTimer;

        VMTDNodeAdapterForm *m_adapterForm;

    private slots:

        void uiTimerTickSlot();

        void pbShowDetailedStateClicked();

        void pbClearErrorsClicked();
    };
}

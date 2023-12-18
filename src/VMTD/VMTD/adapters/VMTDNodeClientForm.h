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

        void sendMessageSignal(QWebSocket *socket, const QJsonObject &messageObj);

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

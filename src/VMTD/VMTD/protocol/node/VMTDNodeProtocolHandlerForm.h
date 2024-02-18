#pragma once

#include "VMTDNodeProtocolHandler.h"
#include "../VMTDProtocolHandlerForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDNodeProtocolHandlerForm;
}

namespace VMTDLib
{
    class VMTDNodeProtocolHandlerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNodeProtocolHandlerForm(QWidget *parent, VMTDNodeProtocolHandler *handler);
        ~VMTDNodeProtocolHandlerForm();

    signals:

        void appendRequestsSignal(const QList<QPair<QString, QJsonObject>> &requests);

    private:

        Ui::VMTDNodeProtocolHandlerForm *ui;

        VMTDNodeProtocolHandler *m_handler;
        VMTDSettings *m_settings;

        VMTDProtocolHandlerForm *m_handlerForm;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void pbSendClicked();
    };
}

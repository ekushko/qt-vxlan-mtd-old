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

        void appendRequestSignal(const QString &method, const QJsonObject &params);

    private:

        Ui::VMTDNodeProtocolHandlerForm *ui;

        VMTDNodeProtocolHandler *m_handler;

        VMTDProtocolHandlerForm *m_handlerForm;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void pbSendClicked();
    };
}

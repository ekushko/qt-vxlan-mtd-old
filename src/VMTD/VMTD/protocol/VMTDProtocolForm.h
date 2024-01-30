#pragma once

#include "VMTDProtocol.h"
#include "VMTDProtocolHandlerForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDProtocolForm;
}

namespace VMTDLib
{
    class VMTDProtocolForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDProtocolForm(QWidget *parent, VMTDProtocol *protocol);
        ~VMTDProtocolForm();

    private:

        Ui::VMTDProtocolForm *ui;

        VMTDProtocol *m_protocol;

        QTimer m_uiTimer;

        QMap<VMTDProtocolHandler *, VMTDProtocolHandlerForm *> m_handlerForms;

    private slots:

        void handlerCreatedSlot(VMTDProtocolHandler *handler);
        void handlerRemovedSlot(VMTDProtocolHandler *handler);

        void uiTimerTickSlot();
    };
}

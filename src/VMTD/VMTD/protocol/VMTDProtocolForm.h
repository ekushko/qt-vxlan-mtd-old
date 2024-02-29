#pragma once

#include "VMTDProtocol.h"
#include "nxapi/VMTDNxApiProtocolHandlerForm.h"
#include "node/VMTDNodeProtocolHandlerForm.h"

#include <QWidget>
#include <QListWidgetItem>

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

        void initializeView();

        void updateNxApiHandlerList();
        void updateNodeHandlerList();

        Ui::VMTDProtocolForm *ui;

        VMTDProtocol *m_protocol;
        VMTDSettings *m_settings;

    private slots:

        void handlerCreatedSlot(VMTDProtocolHandler *handler);
        void handlerRemovedSlot(VMTDProtocolHandler *handler);

        void lwNxApiHandlersItemDoubleClicked(QListWidgetItem *item);
        void lwNodeHandlersItemDoubleClicked(QListWidgetItem *item);
    };
}

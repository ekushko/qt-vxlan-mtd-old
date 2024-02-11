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

        Ui::VMTDProtocolForm *ui;

        VMTDProtocol *m_protocol;
        VMTDSettings *m_settings;

        QMap<VMTDNxApiProtocolHandler *, VMTDNxApiProtocolHandlerForm *> m_nxApiHandlerForms;
        QMap<VMTDNodeProtocolHandler *, VMTDNodeProtocolHandlerForm *> m_nodeHandlerForms;

    private slots:

        void handlerCreatedSlot(VMTDProtocolHandler *handler);
        void handlerRemovedSlot(VMTDProtocolHandler *handler);
    };
}

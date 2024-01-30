#include    "VMTDProtocolForm.h"
#include "ui_VMTDProtocolForm.h"

#include <QTabBar>

namespace VMTDLib
{
    VMTDProtocolForm::VMTDProtocolForm(QWidget *parent, VMTDProtocol *protocol) :
        QWidget(parent),
        ui(new Ui::VMTDProtocolForm),
        m_protocol(protocol)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol");

        connect(m_protocol, &VMTDProtocol::handlerCreatedSignal,
                this, &VMTDProtocolForm::handlerCreatedSlot);
        connect(m_protocol, &VMTDProtocol::handlerRemovedSignal,
                this, &VMTDProtocolForm::handlerRemovedSlot);

        for (auto handler : m_protocol->handlers())
            handlerCreatedSlot(handler);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDProtocolForm::uiTimerTickSlot);
        m_uiTimer.start(200);

        uiTimerTickSlot();
    }

    VMTDProtocolForm::~VMTDProtocolForm()
    {
        delete ui;
    }

    void VMTDProtocolForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        auto form = new VMTDProtocolHandlerForm(ui->tbwHandlers, handler);
        ui->tbwHandlers->addTab(form, handler->name());
        m_handlerForms[handler] = form;
    }
    void VMTDProtocolForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        delete m_handlerForms[handler];
        m_handlerForms.remove(handler);

        for (int i = 0; ui->tbwHandlers->tabBar()->count(); ++i)
        {
            if (ui->tbwHandlers->tabBar()->tabText(i) == handler->name())
            {
                ui->tbwHandlers->removeTab(i);
                break;
            }
        }
    }

    void VMTDProtocolForm::uiTimerTickSlot()
    {
        for (auto form : m_handlerForms.values())
            form->updateView();
    }
}

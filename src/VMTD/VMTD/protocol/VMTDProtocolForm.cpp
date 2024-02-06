#include    "VMTDProtocolForm.h"
#include "ui_VMTDProtocolForm.h"

#include "../VMTDRepo.h"

#include <QTabBar>

namespace VMTDLib
{
    VMTDProtocolForm::VMTDProtocolForm(QWidget *parent, VMTDProtocol *protocol) :
        QWidget(parent),
        ui(new Ui::VMTDProtocolForm),
        m_protocol(protocol)
    {
        m_protocol->settings()->debugOut(VN_S(VMTDProtocolForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol");

        connect(ui->lwNxApi, &QListWidget::itemDoubleClicked,
                this, &VMTDProtocolForm::lwNxApiDoubleClicked);
        connect(ui->lwNode, &QListWidget::itemDoubleClicked,
                this, &VMTDProtocolForm::lwNodeDoubleClicked);

        connect(m_protocol, &VMTDProtocol::handlerCreatedSignal,
                this, &VMTDProtocolForm::handlerCreatedSlot);
        connect(m_protocol, &VMTDProtocol::handlerRemovedSignal,
                this, &VMTDProtocolForm::handlerRemovedSlot);

        for (auto handler : m_protocol->handlers())
            handlerCreatedSlot(handler);

        m_protocol->settings()->debugOut(VN_S(VMTDProtocolForm) + " | Constructor finished");
    }

    VMTDProtocolForm::~VMTDProtocolForm()
    {
        m_protocol->settings()->debugOut(VN_S(VMTDProtocolForm) + " | Destructor called");

        delete ui;

        m_protocol->settings()->debugOut(VN_S(VMTDProtocolForm) + " | Destructor finished");
    }

    void VMTDProtocolForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
            ui->lwNxApi->addItem(handler->name());
        else if (handler->type() == VMTDProtocolHandler::EnType::NODE)
            ui->lwNode->addItem(handler->name());
    }
    void VMTDProtocolForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        QListWidget *lw = nullptr;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
            lw = ui->lwNxApi;
        else if (handler->type() == VMTDProtocolHandler::EnType::NODE)
            lw = ui->lwNode;

        if (lw == nullptr)
            return;

        for (int i = 0; i < lw->count(); ++i)
        {
            if (lw->item(i)->text() == handler->name())
            {
                lw->removeItemWidget(lw->item(i));
                break;
            }
        }
    }

    void VMTDProtocolForm::lwNxApiDoubleClicked(QListWidgetItem *item)
    {
        auto handler = m_protocol->handler(item->text());

        if (handler != nullptr)
            handler->showFormSlot();
    }

    void VMTDProtocolForm::lwNodeDoubleClicked(QListWidgetItem *item)
    {
        auto handler = m_protocol->handler(item->text());

        if (handler != nullptr)
            handler->showFormSlot();
    }
}

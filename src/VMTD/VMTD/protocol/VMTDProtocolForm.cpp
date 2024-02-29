#include    "VMTDProtocolForm.h"
#include "ui_VMTDProtocolForm.h"

#include "../VMTDRepo.h"

#include <QTabBar>

namespace VMTDLib
{
    VMTDProtocolForm::VMTDProtocolForm(QWidget *parent, VMTDProtocol *protocol) :
        QWidget(parent),
        ui(new Ui::VMTDProtocolForm),
        m_protocol(protocol),
        m_settings(protocol->settings())
    {
        m_settings->creationOut(VN_S(VMTDProtocolForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol");

        initializeView();

        connect(m_protocol, &VMTDProtocol::handlerCreatedSignal,
                this, &VMTDProtocolForm::handlerCreatedSlot);
        connect(m_protocol, &VMTDProtocol::handlerRemovedSignal,
                this, &VMTDProtocolForm::handlerRemovedSlot);

        for (auto handler : m_protocol->handlers())
            handlerCreatedSlot(handler);

        m_settings->creationOut(VN_S(VMTDProtocolForm) + " | Constructor finished");
    }

    VMTDProtocolForm::~VMTDProtocolForm()
    {
        m_settings->creationOut(VN_S(VMTDProtocolForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDProtocolForm) + " | Destructor finished");
    }

    void VMTDProtocolForm::initializeView()
    {
        connect(ui->lwNxApiHandlers, &QListWidget::itemDoubleClicked,
                this, &VMTDProtocolForm::lwNxApiHandlersItemDoubleClicked);

        connect(ui->lwNodeHandlers, &QListWidget::itemDoubleClicked,
                this, &VMTDProtocolForm::lwNodeHandlersItemDoubleClicked);
    }

    void VMTDProtocolForm::updateNxApiHandlerList()
    {
        ui->lwNxApiHandlers->clear();

        for (auto handler : m_protocol->nxApiHandlers())
            ui->lwNxApiHandlers->addItem(handler->name());
    }

    void VMTDProtocolForm::updateNodeHandlerList()
    {
        ui->lwNodeHandlers->clear();

        for (auto handler : m_protocol->nodeHandlers())
            ui->lwNodeHandlers->addItem(handler->name());
    }

    void VMTDProtocolForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
            updateNxApiHandlerList();
        else if (handler->type() == VMTDProtocolHandler::EnType::NODE)
            updateNodeHandlerList();
    }
    void VMTDProtocolForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        Q_UNUSED(handler)

        updateNxApiHandlerList();
        updateNodeHandlerList();
    }

    void VMTDProtocolForm::lwNxApiHandlersItemDoubleClicked(QListWidgetItem *item)
    {
        auto index = ui->lwNxApiHandlers->row(item);

        if (index >= m_protocol->nxApiHandlers().size())
            return;

        m_protocol->nxApiHandlers().at(index)->showFormSlot(nullptr);
    }
    void VMTDProtocolForm::lwNodeHandlersItemDoubleClicked(QListWidgetItem *item)
    {
        auto index = ui->lwNodeHandlers->row(item);

        if (index >= m_protocol->nodeHandlers().size())
            return;

        m_protocol->nodeHandlers().at(index)->showFormSlot(nullptr);
    }
}

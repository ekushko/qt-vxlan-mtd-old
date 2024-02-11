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
        m_settings->debugOut(VN_S(VMTDProtocolForm) + " | Destructor called");

        delete ui;

        m_settings->debugOut(VN_S(VMTDProtocolForm) + " | Destructor finished");
    }

    void VMTDProtocolForm::handlerCreatedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
        {
            auto nxApiHandler = dynamic_cast<VMTDNxApiProtocolHandler *>(handler);
            auto form = new VMTDNxApiProtocolHandlerForm(ui->tbwNxApi, nxApiHandler);
            ui->tbwNxApi->addTab(form, nxApiHandler->name());
            m_nxApiHandlerForms[nxApiHandler] = form;
        }
        else if (handler->type() == VMTDProtocolHandler::EnType::NODE)
        {
            auto nodeHandler = dynamic_cast<VMTDNodeProtocolHandler *>(handler);
            auto form = new VMTDNodeProtocolHandlerForm(ui->tbwNode, nodeHandler);
            ui->tbwNode->addTab(form, nodeHandler->name());
            m_nodeHandlerForms[nodeHandler] = form;
        }
    }
    void VMTDProtocolForm::handlerRemovedSlot(VMTDProtocolHandler *handler)
    {
        if (handler == nullptr)
            return;

        if (handler->type() == VMTDProtocolHandler::EnType::NX_API)
        {
            auto nxApiHandler = dynamic_cast<VMTDNxApiProtocolHandler *>(handler);

            delete m_nxApiHandlerForms[nxApiHandler];
            m_nxApiHandlerForms.remove(nxApiHandler);

            for (int i = 0; ui->tbwNxApi->tabBar()->count(); ++i)
            {
                if (ui->tbwNxApi->tabBar()->tabText(i) == nxApiHandler->name())
                {
                    ui->tbwNxApi->removeTab(i);
                    break;
                }
            }
        }
        else if (handler->type() == VMTDProtocolHandler::EnType::NODE)
        {
            auto nodeHandler = dynamic_cast<VMTDNodeProtocolHandler *>(handler);

            delete m_nodeHandlerForms[nodeHandler];
            m_nodeHandlerForms.remove(nodeHandler);

            for (int i = 0; ui->tbwNode->tabBar()->count(); ++i)
            {
                if (ui->tbwNode->tabBar()->tabText(i) == nodeHandler->name())
                {
                    ui->tbwNode->removeTab(i);
                    break;
                }
            }
        }
    }
}

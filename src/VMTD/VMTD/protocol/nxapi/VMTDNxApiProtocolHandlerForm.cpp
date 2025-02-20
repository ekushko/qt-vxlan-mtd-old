#include    "VMTDNxApiProtocolHandlerForm.h"
#include "ui_VMTDNxApiProtocolHandlerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNxApiProtocolHandlerForm::VMTDNxApiProtocolHandlerForm(QWidget *parent,
                                                               VMTDNxApiProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiProtocolHandlerForm),
        m_handler(handler),
        m_settings(handler->settings())
    {
        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandlerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol " + m_handler->name());

        m_handlerForm = new VMTDProtocolHandlerForm(ui->gbProtocolHandler, m_handler);

        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNxApiProtocolHandlerForm::close);
        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDNxApiProtocolHandler::checkConnectionSlot);
        connect(this, &VMTDNxApiProtocolHandlerForm::appendCommandSignal,
                m_handler, &VMTDNxApiProtocolHandler::appendCommandSlot);
        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNxApiProtocolHandlerForm::pbSendClicked);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNxApiProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);

        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandlerForm) + " | Constructor finished");
    }

    VMTDNxApiProtocolHandlerForm::~VMTDNxApiProtocolHandlerForm()
    {
        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandlerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDNxApiProtocolHandlerForm) + " | Destructor finished");
    }

    void VMTDNxApiProtocolHandlerForm::uiTimerTickSlot()
    {
        m_handlerForm->updateView();
    }

    void VMTDNxApiProtocolHandlerForm::pbSendClicked()
    {
        emit appendCommandSignal(ui->pteCommand->toPlainText().split('\n'));

        ui->pteCommand->clear();
    }
}

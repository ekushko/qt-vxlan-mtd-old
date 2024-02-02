#include    "VMTDNxApiProtocolHandlerForm.h"
#include "ui_VMTDNxApiProtocolHandlerForm.h"

namespace VMTDLib
{
    VMTDNxApiProtocolHandlerForm::VMTDNxApiProtocolHandlerForm(QWidget *parent,
                                                               VMTDNxApiProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiProtocolHandlerForm),
        m_handler(handler)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol " + m_handler->name());

        m_handlerForm = new VMTDProtocolHandlerForm(ui->wProtocolHandler, m_handler);

        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDNxApiProtocolHandler::checkConnectionSlot);
        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNxApiProtocolHandlerForm::pbSendClicked);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNxApiProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);
    }

    VMTDNxApiProtocolHandlerForm::~VMTDNxApiProtocolHandlerForm()
    {
        delete ui;
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

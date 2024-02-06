#include    "VMTDNodeProtocolHandlerForm.h"
#include "ui_VMTDNodeProtocolHandlerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeProtocolHandlerForm::VMTDNodeProtocolHandlerForm(QWidget *parent,
                                                             VMTDNodeProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDNodeProtocolHandlerForm),
        m_handler(handler)
    {
        m_handler->settings()->debugOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol " + m_handler->name());

        m_handlerForm = new VMTDProtocolHandlerForm(ui->wProtocolHandler, m_handler);

        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDNodeProtocolHandler::checkConnectionSlot);
        connect(this, &VMTDNodeProtocolHandlerForm::appendRequestSignal,
                m_handler, &VMTDNodeProtocolHandler::appendRequestSlot);
        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNodeProtocolHandlerForm::pbSendClicked);

        if (m_handler->settings()->nodeType() == VMTDNodeType::CLIENT)
            ui->gbRequest->hide();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);

        m_handler->settings()->debugOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Constructor finished");
    }

    VMTDNodeProtocolHandlerForm::~VMTDNodeProtocolHandlerForm()
    {
        m_handler->settings()->debugOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Destructor called");

        delete ui;

        m_handler->settings()->debugOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Destructor finished");
    }

    void VMTDNodeProtocolHandlerForm::uiTimerTickSlot()
    {
        m_handlerForm->updateView();
    }

    void VMTDNodeProtocolHandlerForm::pbSendClicked()
    {
        const auto text = ui->pteParams->toPlainText();
        const auto jsonDoc = QJsonDocument::fromJson(text.toUtf8());

        emit appendRequestSignal(ui->leMethod->text(), jsonDoc.object());
    }
}

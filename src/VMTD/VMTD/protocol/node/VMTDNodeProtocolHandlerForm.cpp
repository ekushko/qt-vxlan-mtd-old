#include    "VMTDNodeProtocolHandlerForm.h"
#include "ui_VMTDNodeProtocolHandlerForm.h"

namespace VMTDLib
{
    VMTDNodeProtocolHandlerForm::VMTDNodeProtocolHandlerForm(QWidget *parent,
                                                             VMTDNodeProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDNodeProtocolHandlerForm),
        m_handler(handler)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol " + m_handler->name());

        m_handlerForm = new VMTDProtocolHandlerForm(ui->wProtocolHandler, m_handler);

        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDNodeProtocolHandler::checkConnectionSlot);
        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNodeProtocolHandlerForm::pbSendClicked);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);
    }

    VMTDNodeProtocolHandlerForm::~VMTDNodeProtocolHandlerForm()
    {
        delete ui;
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

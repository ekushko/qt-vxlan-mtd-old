#include    "VMTDNodeProtocolHandlerForm.h"
#include "ui_VMTDNodeProtocolHandlerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeProtocolHandlerForm::VMTDNodeProtocolHandlerForm(QWidget *parent,
                                                             VMTDNodeProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDNodeProtocolHandlerForm),
        m_handler(handler),
        m_settings(handler->settings())
    {
        m_settings->creationOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Protocol " + m_handler->name());

        m_handlerForm = new VMTDProtocolHandlerForm(ui->gbProtocolHandler, m_handler);

        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNodeProtocolHandlerForm::close);
        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_handler, &VMTDNodeProtocolHandler::checkConnectionSlot);
        connect(this, &VMTDNodeProtocolHandlerForm::appendRequestsSignal,
                m_handler, &VMTDNodeProtocolHandler::appendRequestsSlot);
        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNodeProtocolHandlerForm::pbSendClicked);

        if (m_settings->nodeType() == VMTDNodeType::CLIENT)
            ui->gbRequest->hide();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeProtocolHandlerForm::uiTimerTickSlot);
        m_uiTimer.start(200);

        m_settings->creationOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Constructor finished");
    }

    VMTDNodeProtocolHandlerForm::~VMTDNodeProtocolHandlerForm()
    {
        m_settings->creationOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDNodeProtocolHandlerForm) + " | Destructor finished");
    }

    void VMTDNodeProtocolHandlerForm::uiTimerTickSlot()
    {
        m_handlerForm->updateView();
    }

    void VMTDNodeProtocolHandlerForm::pbSendClicked()
    {
        const auto text = ui->pteParams->toPlainText();
        const auto jsonDoc = QJsonDocument::fromJson(text.toUtf8());

        QList<QPair<QString, QJsonObject>> requests;
        requests.append(qMakePair(ui->leMethod->text(), jsonDoc.object()));
        emit appendRequestsSignal(requests);
    }
}

#include    "VMTDProtocolHandlerForm.h"
#include "ui_VMTDProtocolHandlerForm.h"

namespace VMTDLib
{
    VMTDProtocolHandlerForm::VMTDProtocolHandlerForm(QWidget *parent, VMTDProtocolHandler *handler) :
        QWidget(parent),
        ui(new Ui::VMTDProtocolHandlerForm),
        m_handler(handler)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(m_handler, &VMTDProtocolHandler::showDebugSignal,
                this, &VMTDProtocolHandlerForm::showDebugSlot);

        connect(ui->pbClearQueue, &QPushButton::clicked,
                m_handler, &VMTDProtocolHandler::clearQueueSlot);
        connect(ui->pbClearFlow, &QPushButton::clicked,
                this, &VMTDProtocolHandlerForm::pbClearFlowClicked);

        if (m_handler->settings()->nodeType() == VMTDNodeType::CLIENT)
            ui->lbQueueLength->hide();
    }

    VMTDProtocolHandlerForm::~VMTDProtocolHandlerForm()
    {
        delete ui;
    }

    void VMTDProtocolHandlerForm::updateView()
    {
        ui->lbQueueLength->setText(QString("Queue length: %1").arg(m_handler->queueLength()));
    }

    void VMTDProtocolHandlerForm::showDebugSlot(const QTime &time, const QString &text)
    {
        if (!ui->chbShouldUpdate->isChecked())
            return;

        ui->pteFlow->appendPlainText(QString("[%1] %2\n")
                                     .arg(time.toString("hh:mm:ss:zzz"))
                                     .arg(text));
    }

    void VMTDProtocolHandlerForm::pbClearFlowClicked()
    {
        ui->pteFlow->clear();
    }
}

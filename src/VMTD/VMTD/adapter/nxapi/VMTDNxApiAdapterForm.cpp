#include    "VMTDNxApiAdapterForm.h"
#include "ui_VMTDNxApiAdapterForm.h"

#include "../../VMTDRepo.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNxApiAdapterForm::VMTDNxApiAdapterForm(QWidget *parent, VMTDNxApiAdapter *adapter) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiAdapterForm),
        m_adapter(adapter)
    {
        m_adapter->settings()->debugOut(VN_S(VMTDNxApiAdapterForm) + " was created");

        ui->setupUi(this);

        setWindowTitle(QString("NX-API Адаптер (%1)").arg(m_adapter->url().toString()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(this, &VMTDNxApiAdapterForm::sendCommandSignal,
                m_adapter, &VMTDNxApiAdapter::sendCommandSlot,
                Qt::QueuedConnection);
        connect(m_adapter, &VMTDNxApiAdapter::showDebugSignal,
                this, &VMTDNxApiAdapterForm::showDebugSlot,
                Qt::QueuedConnection);

        initializeView();
        updateView();
    }

    VMTDNxApiAdapterForm::~VMTDNxApiAdapterForm()
    {
        m_adapter->settings()->debugOut(VN_S(VMTDNxApiAdapterForm) + " was deleted");

        delete ui;
    }

    void VMTDNxApiAdapterForm::updateView()
    {
        ui->lbUrl->setText(m_adapter->url().toString());
    }

    void VMTDNxApiAdapterForm::showDebugSlot(const QTime &time, const QString &text)
    {
        if (!ui->chbShouldUpdate->isChecked())
            return;

        ui->pteFlow->appendPlainText(QString("\n[%1] %2\n")
                                     .arg(time.toString("hh:mm:ss:zzz"))
                                     .arg(text));
    }

    void VMTDNxApiAdapterForm::initializeView()
    {
        connect(ui->pbCheckConnection, &QPushButton::clicked,
                m_adapter, &VMTDNxApiAdapter::checkConnectionSlot);
        connect(ui->pbSendCommand, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbSendCommandClicked);
        connect(ui->pbClearFlow, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbClearFlowClicked);
    }

    void VMTDNxApiAdapterForm::pbSendCommandClicked()
    {
        emit sendCommandSignal(ui->pteCommand->toPlainText().split('\n'));

        ui->pteCommand->clear();
    }

    void VMTDNxApiAdapterForm::pbClearFlowClicked()
    {
        ui->pteFlow->clear();
    }
}

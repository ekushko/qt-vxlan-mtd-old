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

        connect(this, &VMTDNxApiAdapterForm::sendCommandsSignal,
                m_adapter, &VMTDNxApiAdapter::sendCommandSlot,
                Qt::QueuedConnection);
        connect(this, &VMTDNxApiAdapterForm::checkConnectionSignal,
                m_adapter, &VMTDNxApiAdapter::checkConnectionSlot,
                Qt::QueuedConnection);
        connect(m_adapter, &VMTDNxApiAdapter::showMessageSignal,
                this, &VMTDNxApiAdapterForm::showMessageSlot,
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
        ui->pbCheckConnection->setEnabled(m_adapter->canSend());
        ui->pbSendCommands->setEnabled(m_adapter->canSend());
        ui->lbConnected->setText(m_adapter->isConnected() ? "Connected" : "Disconnected");
        ui->lbUrl->setText(m_adapter->url().toString());
    }

    void VMTDNxApiAdapterForm::showMessageSlot(const QString &text)
    {
        if (text.isEmpty())
            return;

        ui->pteFlow->appendPlainText("\n-------------------------\n");
        ui->pteFlow->appendPlainText(text);
        ui->pteFlow->appendPlainText("\n-------------------------\n");
    }

    void VMTDNxApiAdapterForm::initializeView()
    {
        connect(ui->pbCheckConnection, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbCheckConnectionClicked);
        connect(ui->pbSendCommands, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbSendCommandsClicked);
    }

    void VMTDNxApiAdapterForm::pbCheckConnectionClicked()
    {
        emit checkConnectionSignal();
    }

    void VMTDNxApiAdapterForm::pbSendCommandsClicked()
    {
        emit sendCommandsSignal(ui->pteCommands->toPlainText().split('\n'));

        ui->pteCommands->clear();
    }
}

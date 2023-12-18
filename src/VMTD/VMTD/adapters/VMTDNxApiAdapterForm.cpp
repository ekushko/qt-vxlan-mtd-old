#include    "VMTDNxApiAdapterForm.h"
#include "ui_VMTDNxApiAdapterForm.h"

#include "../VMTDRepo.h"

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

        initialize();

        connect(&m_quickUiTimer, &QTimer::timeout,
                this, &VMTDNxApiAdapterForm::quickUiTimerTickSlot);
        m_quickUiTimer.start(200);

        connect(&m_uiTimer, &QTimer::timeout,
                this, &VMTDNxApiAdapterForm::uiTimerTickSlot);
        m_quickUiTimer.start(1000);

        setEditMode(false);

        quickUiTimerTickSlot();
        uiTimerTickSlot();
    }

    VMTDNxApiAdapterForm::~VMTDNxApiAdapterForm()
    {
        m_adapter->settings()->debugOut(VN_S(VMTDNxApiAdapterForm) + " was deleted");

        delete ui;
    }

    void VMTDNxApiAdapterForm::showMessageSlot(const QString &text)
    {
        if (text.isEmpty())
            return;

        ui->pteFlow->appendPlainText("\n-------------------------\n");
        ui->pteFlow->appendPlainText(text);
        ui->pteFlow->appendPlainText("\n-------------------------\n");
    }

    void VMTDNxApiAdapterForm::initialize()
    {
        connect(ui->pbHideRight, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbHideRightClicked);
        connect(ui->pbCheckConnection, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbCheckConnectionClicked);
        connect(ui->pbSendCommands, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbSendCommandsClicked);
        connect(ui->pbChange, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked,
                this, &VMTDNxApiAdapterForm::pbCancelClicked);

        connect(this, &VMTDNxApiAdapterForm::sendCommandsSignal,
                m_adapter, &VMTDNxApiAdapter::sendCommandSlot,
                Qt::QueuedConnection);
        connect(this, &VMTDNxApiAdapterForm::checkConnectionSignal,
                m_adapter, &VMTDNxApiAdapter::checkConnectionSlot,
                Qt::QueuedConnection);
        connect(m_adapter, &VMTDNxApiAdapter::showMessageSignal,
                this, &VMTDNxApiAdapterForm::showMessageSlot,
                Qt::QueuedConnection);
    }

    void VMTDNxApiAdapterForm::setEditMode(bool isEditMode)
    {
        ui->leUrl->setEnabled(isEditMode);
        ui->leUserName->setEnabled(isEditMode);
        ui->lePassword->setEnabled(isEditMode);
        ui->sbTicketTimeoutInterval->setEnabled(isEditMode);
    }

    void VMTDNxApiAdapterForm::uiTimerTickSlot()
    {
        ui->leUrl->setText(m_adapter->url().toString());
        ui->leUserName->setText(m_adapter->url().userName());
        ui->lePassword->setText(m_adapter->url().password());
        ui->sbTicketTimeoutInterval->setValue(m_adapter->ticketTimeoutInterval());
    }

    void VMTDNxApiAdapterForm::quickUiTimerTickSlot()
    {
        ui->pbCheckConnection->setEnabled(m_adapter->canSend());
        ui->pbSendCommands->setEnabled(m_adapter->canSend());

        ui->lbIsConnected->setText(m_adapter->isConnected() ? "Есть" : "Нет");
    }

    void VMTDNxApiAdapterForm::pbHideRightClicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbHideRight->setText(ui->wRight->isVisible() ? "<" : ">");
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

    void VMTDNxApiAdapterForm::pbChangeClicked()
    {
        m_uiTimer.stop();
        setEditMode(true);
    }
    void VMTDNxApiAdapterForm::pbAcceptClicked()
    {
        QUrl url(ui->leUrl->text());
        url.setUserName(ui->leUserName->text());
        url.setPassword(ui->lePassword->text());

        m_adapter->setUrl(url);
        m_adapter->setTicketTimeoutInterval(ui->sbTicketTimeoutInterval->value());

        m_uiTimer.start();
        setEditMode(false);
    }
    void VMTDNxApiAdapterForm::pbCancelClicked()
    {
        uiTimerTickSlot();

        m_uiTimer.start();
        setEditMode(false);
    }
}

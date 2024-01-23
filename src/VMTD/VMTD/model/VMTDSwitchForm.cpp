#include    "VMTDSwitchForm.h"
#include "ui_VMTDSwitchForm.h"

namespace VMTDLib
{
    VMTDSwitchForm::VMTDSwitchForm(QWidget *parent, VMTDModel *model, int identificator) :
        QWidget(parent),
        ui(new Ui::VMTDSwitchForm),
        m_model(model),
        m_sw(m_model->sw(identificator)),
        m_identificator(identificator)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Switch (id: %1) [%2]")
                       .arg(m_sw->identificator())
                       .arg(m_sw->url().toString()));

        initializeView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDSwitchForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        setEditMode(false);
        updateView();
    }

    VMTDSwitchForm::~VMTDSwitchForm()
    {
        delete ui;
    }

    void VMTDSwitchForm::initializeView()
    {
        m_portForm = new VMTDSwitchPortForm(ui->wPort, m_model, m_identificator);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDSwitchForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDSwitchForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDSwitchForm::pbCancelClicked);

        connect(m_sw, &VMTDSwitch::portCountChangedSignal, this, &VMTDSwitchForm::portCountChangedSlot);
    }

    void VMTDSwitchForm::setEditMode(bool isEditMode)
    {
        if (isEditMode && m_model->isReadOnly())
            return;

        m_isEditMode = isEditMode;

        ui->pbChange->setEnabled(!m_isEditMode && !m_model->isReadOnly());
        ui->pbAccept->setEnabled(m_isEditMode);
        ui->pbCancel->setEnabled(m_isEditMode);

        ui->tbSwitch->tabBar()->setEnabled(!m_isEditMode);
        ui->wMain->setEnabled(m_isEditMode && ui->tbSwitch->currentIndex() == 0);
        ui->wPort->setEnabled(m_isEditMode && ui->tbSwitch->currentIndex() != 0);
    }

    void VMTDSwitchForm::updateView()
    {
        if (ui->tbSwitch->currentIndex() == 0)
        {
            ui->lbOnline->setText(m_sw->isOnline() ? "Yes" : "No");
            ui->leIdentificator->setText(QString::number(m_identificator));
            ui->leUserName->setText(m_sw->url().userName());
            ui->lePassword->setText(m_sw->url().password());
            ui->leUrl->setText(m_sw->url().toString());
            ui->sbTicketTimeoutInterval->setValue(m_sw->ticketTimeoutInterval());
            ui->sbPortCount->setValue(m_sw->portCount());
        }
        else
        {
            m_portForm->updateView();
        }
    }

    void VMTDSwitchForm::updateData()
    {
        if (ui->tbSwitch->currentIndex() == 0)
        {
            m_identificator = ui->leIdentificator->text().toInt();
            m_sw->setIdentificator(m_identificator);
            auto url = QUrl(ui->leUrl->text());
            url.setUserName(ui->leUserName->text());
            url.setPassword(ui->lePassword->text());
            m_sw->setUrl(url);
            m_sw->setTicketTimeoutInterval(ui->sbTicketTimeoutInterval->value());
            m_sw->setPortCount(ui->sbPortCount->value());
        }
        else
        {
            m_portForm->updateData();
        }
    }

    void VMTDSwitchForm::portCountChangedSlot()
    {
        m_portForm->reinitializeView();
    }

    void VMTDSwitchForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDSwitchForm::pbChangeClicked()
    {
        m_uiTimer.stop();

        setEditMode(true);
        updateView();
    }
    void VMTDSwitchForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
    void VMTDSwitchForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
}

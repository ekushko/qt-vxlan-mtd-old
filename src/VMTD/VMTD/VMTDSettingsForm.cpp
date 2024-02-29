#include    "VMTDSettingsForm.h"
#include "ui_VMTDSettingsForm.h"

#include "VMTDRepo.h"

namespace VMTDLib
{
    VMTDSettingsForm::VMTDSettingsForm(QWidget *parent, VMTDSettings *settings) :
        QWidget(parent),
        ui(new Ui::VMTDSettingsForm),
        m_settings(settings)
    {
        m_settings->creationOut(VN_S(VMTDSettingsForm) + " | Constructor called");

        ui->setupUi(this);

        setWindowTitle(QString("Settings %1").arg(m_settings->systemName()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        CB_FILL_MAP(ui->cbNodeType, VMTDSettings::enNodeTypeToL());

        connect(ui->pbSave, &QPushButton::clicked, this, &VMTDSettingsForm::pbSaveClicked);
        connect(ui->pbLoad, &QPushButton::clicked, this, &VMTDSettingsForm::pbLoadClicked);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDSettingsForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDSettingsForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDSettingsForm::pbCancelClicked);

        setEditMode(false);
        updateView();

        m_settings->creationOut(VN_S(VMTDSettingsForm) + " | Constructor finished");
    }

    VMTDSettingsForm::~VMTDSettingsForm()
    {
        m_settings->creationOut(VN_S(VMTDSettingsForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDSettingsForm) + " | Destructor finished");
    }

    void VMTDSettingsForm::setEditMode(bool isEditMode)
    {
        ui->wFile->setEnabled(!isEditMode);
        ui->wLeft->setEnabled(isEditMode);
        ui->wRight->setEnabled(isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDSettingsForm::updateView()
    {
        CB_SELECT(ui->cbNodeType, m_settings->nodeType());
        ui->lbSystemName->setText(m_settings->systemName());
        ui->leDebugName->setText(m_settings->debugName());
        ui->chbShouldShowDebug->setChecked(m_settings->shouldShowDebug());
        ui->chbShouldShowDebugCreation->setChecked(m_settings->shouldShowDebugCreation());

        ui->sbLocalPort->setValue(m_settings->localPort());

        ui->leServerIp->setText(m_settings->serverIp());
        ui->sbServerPort->setValue(m_settings->serverPort());
        ui->chbShouldReconnect->setChecked(m_settings->shouldReconnect());
        ui->sbReconnectInterval->setValue(m_settings->reconnectInterval());

        ui->chbShouldCheckConnection->setChecked(m_settings->shouldCheckConnection());
        ui->sbCheckConnectionInterval->setValue(m_settings->checkConnectionInterval());
        ui->sbCheckQueueInterval->setValue(m_settings->checkQueueInterval());
        ui->sbTicketTimeoutInterval->setValue(m_settings->ticketTimeoutInterval());

        ui->chbShouldCheckOnline->setChecked(m_settings->shouldCheckOnline());
    }

    void VMTDSettingsForm::updateData()
    {
        m_settings->setNodeType(CB_DATA_TOE(ui->cbNodeType, VMTDNodeType));
        m_settings->setDebugName(ui->leDebugName->text());
        m_settings->setShouldShowDebug(ui->chbShouldShowDebug->isChecked());
        m_settings->setShouldShowDebugCreation(ui->chbShouldShowDebugCreation->isChecked());

        m_settings->setLocalPort(ui->sbLocalPort->value());

        m_settings->setServerIp(ui->leServerIp->text());
        m_settings->setServerPort(ui->sbServerPort->value());
        m_settings->setShouldReconnect(ui->chbShouldReconnect->isChecked());
        m_settings->setReconnectInterval(ui->sbReconnectInterval->value());

        m_settings->setShouldCheckConnection(ui->chbShouldCheckConnection->isChecked());
        m_settings->setCheckConnectionInterval(ui->sbCheckConnectionInterval->value());
        m_settings->setCheckQueueInterval(ui->sbCheckQueueInterval->value());
        m_settings->setTicketTimeoutInterval(ui->sbTicketTimeoutInterval->value());

        m_settings->setShouldCheckOnline(ui->chbShouldCheckOnline->isChecked());
    }

    void VMTDSettingsForm::pbSaveClicked()
    {
        m_settings->save();
    }
    void VMTDSettingsForm::pbLoadClicked()
    {
        m_settings->load();
        m_settings->apply();
    }

    void VMTDSettingsForm::pbChangeClicked()
    {
        setEditMode(true);
    }
    void VMTDSettingsForm::pbAcceptClicked()
    {
        updateData();

        m_settings->apply();
        m_settings->save();

        setEditMode(false);
        updateView();
    }
    void VMTDSettingsForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

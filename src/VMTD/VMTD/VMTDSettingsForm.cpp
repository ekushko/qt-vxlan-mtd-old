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
        m_settings->debugOut(VN_S(VMTDSettingsForm) + " was created");

        ui->setupUi(this);

        setWindowTitle(QString("Настройки %1").arg(m_settings->systemName()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(ui->pbSave, &QPushButton::clicked, this, &VMTDSettingsForm::pbSaveClicked);
        connect(ui->pbLoad, &QPushButton::clicked, this, &VMTDSettingsForm::pbLoadClicked);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDSettingsForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDSettingsForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDSettingsForm::pbCancelClicked);

        setEditMode(false);
        updateView();
    }

    VMTDSettingsForm::~VMTDSettingsForm()
    {
        m_settings->debugOut(VN_S(VMTDSettingsForm) + " was deleted");

        delete ui;
    }

    void VMTDSettingsForm::setEditMode(bool isEditMode)
    {
        ui->wFile->setEnabled(!isEditMode);
        ui->wMain->setEnabled(isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDSettingsForm::updateView()
    {
        ui->lbNodeType->setText(VMTDSettings::enNodeTypeToS(m_settings->nodeType()));
        ui->lbSystemName->setText(m_settings->systemName());
        ui->leDebugName->setText(m_settings->debugName());
        ui->chbShouldShowDebug->setChecked(m_settings->shouldShowDebug());

        ui->leServerIp->setText(m_settings->serverIp());
        ui->sbServerPort->setValue(m_settings->serverPort());
        ui->chbShouldReconnect->setChecked(m_settings->shouldReconnect());
        ui->sbReconnectInterval->setValue(m_settings->reconnectInterval());

        ui->sbLocalPort->setValue(m_settings->localPort());
        ui->chbShouldCheckConnection->setChecked(m_settings->shouldCheckConnection());
        ui->sbCheckConnectionInterval->setValue(m_settings->checkConnectionInterval());
    }

    void VMTDSettingsForm::updateData()
    {
        m_settings->setDebugName(ui->leDebugName->text());
        m_settings->setShouldShowDebug(ui->chbShouldShowDebug->isChecked());

        m_settings->setServerIp(ui->leServerIp->text());
        m_settings->setServerPort(ui->sbServerPort->value());
        m_settings->setShouldReconnect(ui->chbShouldReconnect->isChecked());
        m_settings->setReconnectInterval(ui->sbReconnectInterval->value());

        m_settings->setLocalPort(ui->sbLocalPort->value());
        m_settings->setShouldCheckConnection(ui->chbShouldCheckConnection->isChecked());
        m_settings->setCheckConnectionInterval(ui->sbCheckConnectionInterval->value());
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

#include    "VMTDNxApiDeviceForm.h"
#include "ui_VMTDNxApiDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNxApiDeviceForm::VMTDNxApiDeviceForm(QWidget *parent, VMTDNxApiDevice *device) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiDeviceForm),
        m_device(device),
        m_settings(device->settings())
    {
        m_settings->creationOut(VN_S(VMTDNxApiDeviceForm) + " | Constructor called");

        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("NX-API Device | %1")
                       .arg(m_device->name()));

        initializeView();

        setEditMode(false);
        updateView();

        m_settings->creationOut(VN_S(VMTDNxApiDeviceForm) + " | Constructor finished");
    }

    VMTDNxApiDeviceForm::~VMTDNxApiDeviceForm()
    {
        m_settings->creationOut(VN_S(VMTDNxApiDeviceForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDNxApiDeviceForm) + " | Destructor finished");
    }

    void VMTDNxApiDeviceForm::setEditMode(bool isEditMode)
    {
        ui->wMain->setEnabled(isEditMode);

        ui->pbInterfaces->setEnabled(!isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDNxApiDeviceForm::updateView()
    {
        ui->lbOnline->setText(m_device->isOnline() ? "Yes" : "No");
        ui->lbId->setText(QString::number(m_device->id()));
        ui->leUserName->setText(m_device->url().userName());
        ui->lePassword->setText(m_device->url().password());
        ui->leUrl->setText(m_device->url().toString(QUrl::RemoveUserInfo));
    }

    void VMTDNxApiDeviceForm::updateData()
    {
        auto url = QUrl(ui->leUrl->text());
        url.setUserName(ui->leUserName->text());
        url.setPassword(ui->lePassword->text());
        m_device->setUrl(url);
    }

    void VMTDNxApiDeviceForm::initializeView()
    {
        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNxApiDeviceForm::close);

        connect(ui->pbInterfaces, &QPushButton::clicked,
                m_device->interfaceManager(), &VMTDInterfaceManager::showFormSlot);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDNxApiDeviceForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDNxApiDeviceForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDNxApiDeviceForm::pbCancelClicked);
    }

    void VMTDNxApiDeviceForm::pbChangeClicked()
    {
        setEditMode(true);
        updateView();
    }
    void VMTDNxApiDeviceForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();
    }
    void VMTDNxApiDeviceForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

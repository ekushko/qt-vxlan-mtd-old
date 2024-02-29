#include    "VMTDNodeDeviceForm.h"
#include "ui_VMTDNodeDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeDeviceForm::VMTDNodeDeviceForm(QWidget *parent, VMTDNodeDevice *device) :
        QWidget(parent),
        ui(new Ui::VMTDNodeDeviceForm),
        m_device(device),
        m_settings(device->settings())
    {
        m_settings->creationOut(VN_S(VMTDNodeDeviceForm) + " | Constructor called");

        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Node Device | %1")
                       .arg(m_device->name()));

        initializeView();

        setEditMode(false);
        updateView();

        m_settings->creationOut(VN_S(VMTDNodeDeviceForm) + " | Constructor finished");
    }

    VMTDNodeDeviceForm::~VMTDNodeDeviceForm()
    {
        m_settings->creationOut(VN_S(VMTDNodeDeviceForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDNodeDeviceForm) + " | Destructor finished");
    }

    void VMTDNodeDeviceForm::setEditMode(bool isEditMode)
    {
        ui->wMain->setEnabled(isEditMode);

        ui->pbInterfaces->setEnabled(!isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDNodeDeviceForm::updateView()
    {
        ui->lbOnline->setText(m_device->isOnline() ? "Yes" : "No");
        ui->lbId->setText(QString::number(m_device->id()));
        ui->leIp->setText(m_device->ip());
        ui->leDomainName->setText(m_device->domainName());
    }

    void VMTDNodeDeviceForm::updateData()
    {
        m_device->setIp(ui->leIp->text());
        m_device->setDomainName(ui->leDomainName->text());
    }

    void VMTDNodeDeviceForm::initializeView()
    {
        connect(ui->pbInterfaces, &QPushButton::clicked,
                m_device->interfaceManager(), &VMTDInterfaceManager::showFormSlot);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDNodeDeviceForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDNodeDeviceForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDNodeDeviceForm::pbCancelClicked);
    }

    void VMTDNodeDeviceForm::pbChangeClicked()
    {
        setEditMode(true);
        updateView();
    }
    void VMTDNodeDeviceForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();
    }
    void VMTDNodeDeviceForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

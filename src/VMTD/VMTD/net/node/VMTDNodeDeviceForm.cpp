#include    "VMTDNodeDeviceForm.h"
#include "ui_VMTDNodeDeviceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeDeviceForm::VMTDNodeDeviceForm(QWidget *parent, VMTDNodeDevice *device) :
        QWidget(parent),
        ui(new Ui::VMTDNodeDeviceForm),
        m_device(device)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Node Device (%1)")
                       .arg(m_device->name()));

        initializeView();

        setEditMode(false);
        updateView();
    }

    VMTDNodeDeviceForm::~VMTDNodeDeviceForm()
    {
        delete ui;
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
        CB_SELECT(ui->cbRole, m_device->role());
        ui->leIp->setText(m_device->ip());
    }

    void VMTDNodeDeviceForm::updateData()
    {
        m_device->setRole(CB_DATA_TOE(ui->cbRole, VMTDNodeDevice::EnRole));
        m_device->setIp(ui->leIp->text());
    }

    void VMTDNodeDeviceForm::initializeView()
    {
        CB_FILL_MAP(ui->cbRole, VMTDNodeDevice::enRoleToL());

        connect(ui->pbInterfaces, &QPushButton::clicked,
                m_device->interfaces(), &VMTDInterfaces::showFormSlot);

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

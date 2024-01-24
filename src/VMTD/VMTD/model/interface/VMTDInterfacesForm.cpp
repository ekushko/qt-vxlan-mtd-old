#include    "VMTDInterfacesForm.h"
#include "ui_VMTDInterfacesForm.h"

#include "../../VMTDRepo.h"

#include <algorithm>

namespace VMTDLib
{
    VMTDInterfacesForm::VMTDInterfacesForm(QWidget *parent, VMTDInterfaces *interfaces) :
        QWidget(parent),
        ui(new Ui::VMTDInterfacesForm),
        m_interfaces(interfaces)
    {
        ui->setupUi(this);
    }

    VMTDInterfacesForm::~VMTDInterfacesForm()
    {
        delete ui;
    }

    void VMTDInterfacesForm::setEditMode(bool isEditMode)
    {
        ui->wInterfaces->setEnabled(isEditMode);

        ui->wEdit->setEnabled(isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDInterfacesForm::interfaceCreatedSlot(int id)
    {

    }
    void VMTDInterfacesForm::interfaceRemovedSlot(int id)
    {

    }

    void VMTDInterfacesForm::pbAddClicked()
    {
        m_interfaces->addInterface(VMTDRepo::generateId());
    }
    void VMTDInterfacesForm::pbRemoveClicked()
    {
        m_interfaces->removeInterface(m_interfaceForms.last()->id());
    }

    void VMTDInterfacesForm::pbChangeClicked()
    {
        setEditMode(true);
        updateView();
    }
    void VMTDInterfacesForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();
    }
    void VMTDInterfacesForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

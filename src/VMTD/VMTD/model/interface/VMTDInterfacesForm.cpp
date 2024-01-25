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

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Interfaces");

        initializeView();

        setEditMode(false);
        updateView();
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

    void VMTDInterfacesForm::updateView()
    {
        for (auto form : m_interfaceForms)
            form->updateView();
    }

    void VMTDInterfacesForm::updateData()
    {
        for (auto form : m_interfaceForms)
            form->updateData();
    }

    void VMTDInterfacesForm::initializeView()
    {
        for (auto interface : m_interfaces->interfaces())
        {
            auto form = new VMTDInterfaceForm(ui->wInterfaces, interface);
            m_interfaceForms.append(form);
        }
    }

    void VMTDInterfacesForm::interfaceCreatedSlot(int id)
    {
        auto form = new VMTDInterfaceForm(ui->wInterfaces, m_interfaces->interface(id));
        m_interfaceForms.append(form);

        ui->pbAdd->setEnabled(!m_interfaces->onlyOneMode() || m_interfaces->interfaces().size() == 0);
    }
    void VMTDInterfacesForm::interfaceRemovedSlot(int id)
    {
        auto res = std::find_if(m_interfaceForms.begin(), m_interfaceForms.end(),
                                [id](VMTDInterfaceForm * form)
        {
            return form->id() == id;
        });

        if (res != m_interfaceForms.end())
        {
            delete *res;
            m_interfaceForms.erase(res);
        }

        ui->pbRemove->setEnabled(m_interfaces->interfaces().size() != 0);
        ui->pbAdd->setEnabled(m_interfaces->interfaces().size() == 0);
    }

    void VMTDInterfacesForm::pbAddClicked()
    {
        m_interfaces->addInterface();
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

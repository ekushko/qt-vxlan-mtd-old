#include    "VMTDInterfaceManagerForm.h"
#include "ui_VMTDInterfaceManagerForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDInterfaceManagerForm::VMTDInterfaceManagerForm(QWidget *parent, VMTDInterfaceManager *manager) :
        QWidget(parent),
        ui(new Ui::VMTDInterfaceManagerForm),
        m_manager(manager),
        m_settings(manager->settings())
    {
        m_settings->creationOut(VN_S(VMTDInterfaceManagerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Interfaces");

        initializeView();

        setEditMode(false);
        updateView();

        m_settings->creationOut(VN_S(VMTDInterfaceManagerForm) + " | Constructor finished");
    }

    VMTDInterfaceManagerForm::~VMTDInterfaceManagerForm()
    {
        m_settings->creationOut(VN_S(VMTDInterfaceManagerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDInterfaceManagerForm) + " | Destructor finished");
    }

    void VMTDInterfaceManagerForm::setEditMode(bool isEditMode)
    {
        ui->wInterfaces->setEnabled(isEditMode);

        ui->wEdit->setEnabled(!isEditMode);

        ui->pbChange->setEnabled(!isEditMode);
        ui->pbAccept->setEnabled(isEditMode);
        ui->pbCancel->setEnabled(isEditMode);
    }

    void VMTDInterfaceManagerForm::updateView()
    {
        for (auto form : m_interfaceForms)
            form->updateView();
    }

    void VMTDInterfaceManagerForm::updateData()
    {
        for (auto form : m_interfaceForms)
            form->updateData();
    }

    void VMTDInterfaceManagerForm::initializeView()
    {
        for (auto interface : m_manager->interfaces())
        {
            auto form = new VMTDInterfaceForm(ui->wInterfaces, interface);
            m_interfaceForms[form->id()] = form;
        }

        connect(m_manager, &VMTDInterfaceManager::interfaceCreatedSignal,
                this, &VMTDInterfaceManagerForm::interfaceCreatedSlot);
        connect(m_manager, &VMTDInterfaceManager::interfaceRemovedSignal,
                this, &VMTDInterfaceManagerForm::interfaceRemovedSlot);

        connect(ui->pbAdd, &QPushButton::clicked, this, &VMTDInterfaceManagerForm::pbAddClicked);
        connect(ui->pbRemove, &QPushButton::clicked, this, &VMTDInterfaceManagerForm::pbRemoveClicked);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDInterfaceManagerForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDInterfaceManagerForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDInterfaceManagerForm::pbCancelClicked);
    }

    void VMTDInterfaceManagerForm::interfaceCreatedSlot(int id)
    {
        auto form = new VMTDInterfaceForm(ui->wInterfaces, m_manager->interface(id));
        m_interfaceForms[form->id()] = form;

        ui->pbAdd->setEnabled(!m_manager->onlyOneMode() || m_manager->interfaces().size() == 0);
        ui->pbRemove->setEnabled(m_manager->interfaces().size() > 0);
    }
    void VMTDInterfaceManagerForm::interfaceRemovedSlot(int id)
    {
        if (!m_interfaceForms.contains(id))
            return;

        delete m_interfaceForms[id];
        m_interfaceForms.remove(id);

        ui->pbAdd->setEnabled(!m_manager->onlyOneMode() || m_manager->interfaces().size() == 0);
        ui->pbRemove->setEnabled(m_manager->interfaces().size() > 1);
    }

    void VMTDInterfaceManagerForm::pbAddClicked()
    {
        m_manager->addInterface();
    }
    void VMTDInterfaceManagerForm::pbRemoveClicked()
    {
        if (m_interfaceForms.isEmpty())
            return;

        m_manager->removeInterface(m_interfaceForms.last()->id());
    }

    void VMTDInterfaceManagerForm::pbChangeClicked()
    {
        setEditMode(true);
        updateView();
    }
    void VMTDInterfaceManagerForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();
    }
    void VMTDInterfaceManagerForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

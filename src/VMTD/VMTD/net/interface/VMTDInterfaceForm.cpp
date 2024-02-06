#include    "VMTDInterfaceForm.h"
#include "ui_VMTDInterfaceForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDInterfaceForm::VMTDInterfaceForm(QWidget *parent, VMTDInterface *interface) :
        QWidget(parent),
        ui(new Ui::VMTDInterfaceForm),
        m_interface(interface),
        m_settings(interface->settings())
    {
        m_settings->debugOut(VN_S(VMTDInterfaceForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        updateView();

        m_settings->debugOut(VN_S(VMTDInterfaceForm) + " | Constructor finished");
    }

    VMTDInterfaceForm::~VMTDInterfaceForm()
    {
        m_settings->debugOut(VN_S(VMTDInterfaceForm) + " | Destructor called");

        delete ui;

        m_settings->debugOut(VN_S(VMTDInterfaceForm) + " | Destructor finished");
    }

    int VMTDInterfaceForm::id() const
    {
        return m_interface->id();
    }

    void VMTDInterfaceForm::updateView()
    {
        ui->lbId->setText(QString::number(m_interface->id()));
        ui->leName->setText(m_interface->name());
        ui->leIp->setText(m_interface->ip());
    }

    void VMTDInterfaceForm::updateData()
    {
        m_interface->setName(ui->leName->text());
        m_interface->setIp(ui->leIp->text());
    }
}

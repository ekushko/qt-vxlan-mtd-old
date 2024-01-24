#include    "VMTDInterfaceForm.h"
#include "ui_VMTDInterfaceForm.h"

namespace VMTDLib
{
    VMTDInterfaceForm::VMTDInterfaceForm(QWidget *parent, VMTDInterface *interface) :
        QWidget(parent),
        ui(new Ui::VMTDInterfaceForm),
        m_interface(interface)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);
    }

    VMTDInterfaceForm::~VMTDInterfaceForm()
    {
        delete ui;
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

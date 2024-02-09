#include    "VMTDConnectionForm.h"
#include "ui_VMTDConnectionForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDConnectionForm::VMTDConnectionForm(QWidget *parent,
                                           VMTDDevice *device_1, VMTDInterface *interface_1,
                                           VMTDDevice *device_2, VMTDInterface *interface_2) :
        QWidget(parent),
        ui(new Ui::VMTDConnectionForm),
        m_settings(device_1->settings()),
        m_device_1(device_1),
        m_interface_1(interface_1),
        m_device_2(device_2),
        m_interface_2(interface_2)
    {
        m_settings->debugOut(VN_S(VMTDConnectionForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        initializeView();

        m_settings->debugOut(VN_S(VMTDConnectionForm) + " | Constructor finished");
    }

    VMTDConnectionForm::~VMTDConnectionForm()
    {
        m_settings->debugOut(VN_S(VMTDConnectionForm) + " | Destructor called");

        delete ui;

        m_settings->debugOut(VN_S(VMTDConnectionForm) + " | Destructor finished");
    }

    VMTDDevice *VMTDConnectionForm::device_1() const
    {
        return m_device_1;
    }
    VMTDInterface *VMTDConnectionForm::interface_1() const
    {
        return m_interface_1;
    }

    VMTDDevice *VMTDConnectionForm::device_2() const
    {
        return m_device_2;
    }
    VMTDInterface *VMTDConnectionForm::interface_2() const
    {
        return m_interface_2;
    }

    void VMTDConnectionForm::initializeView()
    {
        ui->lbDevice_1->setText(QString("%1 iface id: %1 [%2]")
                                .arg(m_device_1->name())
                                .arg(m_interface_1->id())
                                .arg(m_interface_1->name()));

        ui->lbDevice_2->setText(QString("%1 iface id: %1 [%2]")
                                .arg(m_device_2->name())
                                .arg(m_interface_2->id())
                                .arg(m_interface_2->name()));

        connect(ui->pbRemove, &QPushButton::clicked,
                this, &VMTDConnectionForm::removeSignal);
    }
}

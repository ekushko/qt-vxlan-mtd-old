#include    "VMTDConfiguratorForm.h"
#include "ui_VMTDConfiguratorForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDConfiguratorForm::VMTDConfiguratorForm(QWidget *parent, VMTDConfigurator *configurator) :
        QWidget(parent),
        ui(new Ui::VMTDConfiguratorForm),
        m_configurator(configurator),
        m_settings(configurator->settings())
    {
        m_settings->creationOut(VN_S(VMTDConfiguratorForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Configurator");

        initializeView();
        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDConfiguratorForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        m_settings->creationOut(VN_S(VMTDConfiguratorForm) + " | Constructor finished");
    }

    VMTDConfiguratorForm::~VMTDConfiguratorForm()
    {
        m_settings->creationOut(VN_S(VMTDConfiguratorForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDConfiguratorForm) + " | Destructor finished");
    }

    void VMTDConfiguratorForm::initializeView()
    {
        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDConfiguratorForm::close);
    }

    void VMTDConfiguratorForm::updateView()
    {
        ui->lbNetplan1->setText(m_configurator->netplan1());
        ui->lbNetplan2->setText(m_configurator->netplan2());
        ui->lbHosts->setText(m_configurator->hosts());
    }

    void VMTDConfiguratorForm::uiTimerTickSlot()
    {
        updateView();
    }
}

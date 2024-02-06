#include    "VMTDNetForm.h"
#include "ui_VMTDNetForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNetForm::VMTDNetForm(QWidget *parent, VMTDNet *net) :
        QWidget(parent),
        ui(new Ui::VMTDNetForm),
        m_net(net),
        m_settings(net->settings())
    {
        m_settings->debugOut(VN_S(VMTDNetForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(ui->pbSave, &QPushButton::clicked,
                m_net, &VMTDNet::saveSlot);
        connect(ui->pbLoad, &QPushButton::clicked,
                m_net, &VMTDNet::loadSlot);

        connect(ui->pbAddNxApiDevice, &QPushButton::clicked,
                this, &VMTDNetForm::pbAddNxApiDeviceClicked);
        connect(ui->pbRemoveNxApiDevice, &QPushButton::clicked,
                this, &VMTDNetForm::pbRemoveNxApiDeviceClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemClicked,
                this, &VMTDNetForm::lwNxApiDevicesItemClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDNetForm::lwNxApiDevicesItemDoubleClicked);

        connect(ui->pbAddNodeDevice, &QPushButton::clicked,
                this, &VMTDNetForm::pbAddNodeDeviceClicked);
        connect(ui->pbRemoveNodeDevice, &QPushButton::clicked,
                this, &VMTDNetForm::pbRemoveNodeDeviceClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemClicked,
                this, &VMTDNetForm::lwNodeDevicesItemClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDNetForm::lwNodeDevicesItemDoubleClicked);

        updateNxApiDevicesList();
        updateNodeDevicesList();

        m_settings->debugOut(VN_S(VMTDNetForm) + " | Constructor finished");
    }

    VMTDNetForm::~VMTDNetForm()
    {
        m_settings->debugOut(VN_S(VMTDNetForm) + " | Destructor called");

        delete ui;

        m_settings->debugOut(VN_S(VMTDNetForm) + " | Destructor finished");
    }

    void VMTDNetForm::updateNxApiDevicesList()
    {
        ui->lwNxApiDevices->clear();

        for (auto nxApiDevice : m_net->nxApiDevices().values())
            ui->lwNxApiDevices->addItem(nxApiDevice->name());
    }

    void VMTDNetForm::updateNodeDevicesList()
    {
        ui->lwNodeDevices->clear();

        for (auto nodeDevice : m_net->nodeDevices().values())
        {
            const auto label = QString("id: %1 [%2]")
                               .arg(nodeDevice->id())
                               .arg(nodeDevice->ip());
            ui->lwNodeDevices->addItem(label);
        }
    }

    void VMTDNetForm::lwNxApiDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNxApiDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDNetForm::lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNxApiDevicesItemClicked(item);

        auto nxApiDevice = m_net->nxApiDevice(m_currentNxApiDeviceId);
        nxApiDevice->showFormSlot();
    }
    void VMTDNetForm::pbAddNxApiDeviceClicked()
    {
        if (m_net->addNxApiDevice())
            updateNxApiDevicesList();
    }
    void VMTDNetForm::pbRemoveNxApiDeviceClicked()
    {
        if (m_net->removeNxApiDevice(m_currentNxApiDeviceId))
            updateNxApiDevicesList();
    }

    void VMTDNetForm::lwNodeDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNodeDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDNetForm::lwNodeDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNodeDevicesItemClicked(item);

        auto nodeDevice = m_net->nodeDevice(m_currentNodeDeviceId);
        nodeDevice->showFormSlot();
    }
    void VMTDNetForm::pbAddNodeDeviceClicked()
    {
        if (m_net->addNodeDevice())
            updateNodeDevicesList();
    }
    void VMTDNetForm::pbRemoveNodeDeviceClicked()
    {
        if (m_net->removeNodeDevice(m_currentNodeDeviceId))
            updateNodeDevicesList();
    }
}

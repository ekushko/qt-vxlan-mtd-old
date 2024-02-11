#include    "VMTDDeviceManagerForm.h"
#include "ui_VMTDDeviceManagerForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDDeviceManagerForm::VMTDDeviceManagerForm(QWidget *parent, VMTDDeviceManager *net) :
        QWidget(parent),
        ui(new Ui::VMTDDeviceManagerForm),
        m_net(net),
        m_settings(net->settings())
    {
        m_settings->creationOut(VN_S(VMTDDeviceManagerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(ui->pbSave, &QPushButton::clicked,
                m_net, &VMTDDeviceManager::saveSlot);
        connect(ui->pbLoad, &QPushButton::clicked,
                m_net, &VMTDDeviceManager::loadSlot);

        connect(ui->pbAddNxApiDevice, &QPushButton::clicked,
                this, &VMTDDeviceManagerForm::pbAddNxApiDeviceClicked);
        connect(ui->pbRemoveNxApiDevice, &QPushButton::clicked,
                this, &VMTDDeviceManagerForm::pbRemoveNxApiDeviceClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemClicked,
                this, &VMTDDeviceManagerForm::lwNxApiDevicesItemClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDDeviceManagerForm::lwNxApiDevicesItemDoubleClicked);

        connect(ui->pbAddNodeDevice, &QPushButton::clicked,
                this, &VMTDDeviceManagerForm::pbAddNodeDeviceClicked);
        connect(ui->pbRemoveNodeDevice, &QPushButton::clicked,
                this, &VMTDDeviceManagerForm::pbRemoveNodeDeviceClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemClicked,
                this, &VMTDDeviceManagerForm::lwNodeDevicesItemClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDDeviceManagerForm::lwNodeDevicesItemDoubleClicked);

        updateNxApiDevicesList();
        updateNodeDevicesList();

        m_settings->creationOut(VN_S(VMTDDeviceManagerForm) + " | Constructor finished");
    }

    VMTDDeviceManagerForm::~VMTDDeviceManagerForm()
    {
        m_settings->creationOut(VN_S(VMTDDeviceManagerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDDeviceManagerForm) + " | Destructor finished");
    }

    void VMTDDeviceManagerForm::updateNxApiDevicesList()
    {
        ui->lwNxApiDevices->clear();

        for (auto nxApiDevice : m_net->nxApiDevices().values())
            ui->lwNxApiDevices->addItem(nxApiDevice->name());
    }

    void VMTDDeviceManagerForm::updateNodeDevicesList()
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

    void VMTDDeviceManagerForm::lwNxApiDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNxApiDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDDeviceManagerForm::lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNxApiDevicesItemClicked(item);

        auto nxApiDevice = m_net->nxApiDevice(m_currentNxApiDeviceId);
        nxApiDevice->showFormSlot();
    }
    void VMTDDeviceManagerForm::pbAddNxApiDeviceClicked()
    {
        if (m_net->addNxApiDevice())
            updateNxApiDevicesList();
    }
    void VMTDDeviceManagerForm::pbRemoveNxApiDeviceClicked()
    {
        if (m_net->removeNxApiDevice(m_currentNxApiDeviceId))
            updateNxApiDevicesList();
    }

    void VMTDDeviceManagerForm::lwNodeDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNodeDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDDeviceManagerForm::lwNodeDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNodeDevicesItemClicked(item);

        auto nodeDevice = m_net->nodeDevice(m_currentNodeDeviceId);
        nodeDevice->showFormSlot();
    }
    void VMTDDeviceManagerForm::pbAddNodeDeviceClicked()
    {
        if (m_net->addNodeDevice())
            updateNodeDevicesList();
    }
    void VMTDDeviceManagerForm::pbRemoveNodeDeviceClicked()
    {
        if (m_net->removeNodeDevice(m_currentNodeDeviceId))
            updateNodeDevicesList();
    }
}

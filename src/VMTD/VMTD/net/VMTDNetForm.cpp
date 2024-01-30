#include    "VMTDNetForm.h"
#include "ui_VMTDNetForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNetForm::VMTDNetForm(QWidget *parent, VMTDNet *model) :
        QWidget(parent),
        ui(new Ui::VMTDNetForm),
        m_model(model)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(ui->pbSave, &QPushButton::clicked,
                m_model, &VMTDNet::saveSlot);
        connect(ui->pbLoad, &QPushButton::clicked,
                m_model, &VMTDNet::loadSlot);

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
    }

    VMTDNetForm::~VMTDNetForm()
    {
        delete ui;
    }

    void VMTDNetForm::updateNxApiDevicesList()
    {
        ui->lwNxApiDevices->clear();

        for (auto nxApiDevice : m_model->nxApiDevices().values())
        {
            const auto label = QString("id: %1 [%2]")
                               .arg(nxApiDevice->id())
                               .arg(nxApiDevice->url().toString(QUrl::RemoveUserInfo));
            ui->lwNxApiDevices->addItem(label);
        }
    }

    void VMTDNetForm::updateNodeDevicesList()
    {
        ui->lwNodeDevices->clear();

        for (auto nodeDevice : m_model->nodeDevices().values())
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

        auto nxApiDevice = m_model->nxApiDevice(m_currentNxApiDeviceId);
        nxApiDevice->showFormSlot();
    }
    void VMTDNetForm::pbAddNxApiDeviceClicked()
    {
        if (m_model->addNxApiDevice())
            updateNxApiDevicesList();
    }
    void VMTDNetForm::pbRemoveNxApiDeviceClicked()
    {
        if (m_model->removeNxApiDevice(m_currentNxApiDeviceId))
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

        auto nodeDevice = m_model->nodeDevice(m_currentNodeDeviceId);
        nodeDevice->showFormSlot();
    }
    void VMTDNetForm::pbAddNodeDeviceClicked()
    {
        if (m_model->addNodeDevice())
            updateNodeDevicesList();
    }
    void VMTDNetForm::pbRemoveNodeDeviceClicked()
    {
        if (m_model->removeNodeDevice(m_currentNodeDeviceId))
            updateNodeDevicesList();
    }
}

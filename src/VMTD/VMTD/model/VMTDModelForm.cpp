#include    "VMTDModelForm.h"
#include "ui_VMTDModelForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDModelForm::VMTDModelForm(QWidget *parent, VMTDModel *model) :
        QWidget(parent),
        ui(new Ui::VMTDModelForm),
        m_model(model)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        connect(ui->pbSave, &QPushButton::clicked,
                m_model, &VMTDModel::saveSlot);
        connect(ui->pbLoad, &QPushButton::clicked,
                m_model, &VMTDModel::loadSlot);

        connect(ui->pbAddNxApiDevice, &QPushButton::clicked,
                this, &VMTDModelForm::pbAddNxApiDeviceClicked);
        connect(ui->pbRemoveNxApiDevice, &QPushButton::clicked,
                this, &VMTDModelForm::pbRemoveNxApiDeviceClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemClicked,
                this, &VMTDModelForm::lwNxApiDevicesItemClicked);
        connect(ui->lwNxApiDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDModelForm::lwNxApiDevicesItemDoubleClicked);

        connect(ui->pbAddNodeDevice, &QPushButton::clicked,
                this, &VMTDModelForm::pbAddNodeDeviceClicked);
        connect(ui->pbRemoveNodeDevice, &QPushButton::clicked,
                this, &VMTDModelForm::pbRemoveNodeDeviceClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemClicked,
                this, &VMTDModelForm::lwNodeDevicesItemClicked);
        connect(ui->lwNodeDevices, &QListWidget::itemDoubleClicked,
                this, &VMTDModelForm::lwNodeDevicesItemDoubleClicked);

        updateNxApiDevicesList();
        updateNodeDevicesList();
    }

    VMTDModelForm::~VMTDModelForm()
    {
        delete ui;
    }

    void VMTDModelForm::updateNxApiDevicesList()
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

    void VMTDModelForm::updateNodeDevicesList()
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

    void VMTDModelForm::lwNxApiDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNxApiDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDModelForm::lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNxApiDevicesItemClicked(item);

        auto nxApiDevice = m_model->nxApiDevice(m_currentNxApiDeviceId);
        nxApiDevice->showFormSlot();
    }
    void VMTDModelForm::pbAddNxApiDeviceClicked()
    {
        if (m_model->addNxApiDevice())
            updateNxApiDevicesList();
    }
    void VMTDModelForm::pbRemoveNxApiDeviceClicked()
    {
        if (m_model->removeNxApiDevice(m_currentNxApiDeviceId))
            updateNxApiDevicesList();
    }

    void VMTDModelForm::lwNodeDevicesItemClicked(QListWidgetItem *item)
    {
        const auto label = item->text();

        int firstSpacePos = label.indexOf(' ');
        int secondSpacePos = label.indexOf(' ', firstSpacePos + 1);

        m_currentNodeDeviceId = label.mid(firstSpacePos + 1, secondSpacePos - (firstSpacePos + 1)).toInt();
    }
    void VMTDModelForm::lwNodeDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        lwNodeDevicesItemClicked(item);

        auto nodeDevice = m_model->nodeDevice(m_currentNodeDeviceId);
        nodeDevice->showFormSlot();
    }
    void VMTDModelForm::pbAddNodeDeviceClicked()
    {
        if (m_model->addNodeDevice())
            updateNodeDevicesList();
    }
    void VMTDModelForm::pbRemoveNodeDeviceClicked()
    {
        if (m_model->removeNodeDevice(m_currentNodeDeviceId))
            updateNodeDevicesList();
    }
}

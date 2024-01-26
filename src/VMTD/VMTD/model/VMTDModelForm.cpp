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

        for (auto sw : m_model->nxApiDevices().values())
            ui->lwNxApiDevices->addItem(sw->url().toString(QUrl::RemoveUserInfo));
    }

    void VMTDModelForm::updateNodeDevicesList()
    {
        ui->lwNodeDevices->clear();

        for (auto node : m_model->nodeDevices().values())
            ui->lwNodeDevices->addItem(node->ip());
    }

    void VMTDModelForm::lwNxApiDevicesItemClicked(QListWidgetItem *item)
    {
        m_currentUrl = QUrl(item->text());
    }
    void VMTDModelForm::lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        auto nxApiDevice = m_model->nxApiDevice(QUrl(item->text()));
        nxApiDevice->showFormSlot();
    }
    void VMTDModelForm::pbAddNxApiDeviceClicked()
    {
        if (m_model->addNxApiDevice())
            updateNxApiDevicesList();
    }
    void VMTDModelForm::pbRemoveNxApiDeviceClicked()
    {
        if (m_model->removeNxApiDevice(m_model->nxApiDevice(m_currentUrl)->id()))
            updateNxApiDevicesList();
    }

    void VMTDModelForm::lwNodeDevicesItemClicked(QListWidgetItem *item)
    {
        m_currentIp = item->text();
    }
    void VMTDModelForm::lwNodeDevicesItemDoubleClicked(QListWidgetItem *item)
    {
        auto node = m_model->nodeDevice(item->text());
        node->showFormSlot();
    }
    void VMTDModelForm::pbAddNodeDeviceClicked()
    {
        if (m_model->addNodeDevice())
            updateNodeDevicesList();
    }
    void VMTDModelForm::pbRemoveNodeDeviceClicked()
    {
        if (m_model->removeNodeDevice(m_model->nodeDevice(m_currentIp)->id()))
            updateNodeDevicesList();
    }
}

#include    "VMTDModelForm.h"
#include "ui_VMTDModelForm.h"

#include "VMTDNodeForm.h"
#include "VMTDSwitchForm.h"

#include "../VMTDRepo.h"

#include <QDebug>

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

        connect(ui->pbAddSwitch, &QPushButton::clicked,
                this, &VMTDModelForm::pbAddSwitchClicked);
        connect(ui->pbRemoveSwitch, &QPushButton::clicked,
                this, &VMTDModelForm::pbRemoveSwitchClicked);
        connect(ui->lwSwitches, &QListWidget::itemClicked,
                this, &VMTDModelForm::lwSwitchesItemClicked);
        connect(ui->lwSwitches, &QListWidget::itemDoubleClicked,
                this, &VMTDModelForm::lwSwitchesItemDoubleClicked);

        connect(ui->pbAddNode, &QPushButton::clicked,
                this, &VMTDModelForm::pbAddNodeClicked);
        connect(ui->pbRemoveNode, &QPushButton::clicked,
                this, &VMTDModelForm::pbRemoveNodeClicked);
        connect(ui->lwNodes, &QListWidget::itemClicked,
                this, &VMTDModelForm::lwNodesItemClicked);
        connect(ui->lwNodes, &QListWidget::itemDoubleClicked,
                this, &VMTDModelForm::lwNodesItemDoubleClicked);

        updateSwitchesList();
        updateNodesList();
    }

    VMTDModelForm::~VMTDModelForm()
    {
        delete ui;
    }

    void VMTDModelForm::updateSwitchesList()
    {
        ui->lwSwitches->clear();

        for (auto sw : m_model->switches().values())
            ui->lwSwitches->addItem(sw->url().toString());
    }

    void VMTDModelForm::updateNodesList()
    {
        ui->lwNodes->clear();

        for (auto node : m_model->nodes().values())
            ui->lwNodes->addItem(node->ip());
    }

    void VMTDModelForm::lwSwitchesItemClicked(QListWidgetItem *item)
    {
        m_currentUrl = QUrl(item->text());
    }
    void VMTDModelForm::lwSwitchesItemDoubleClicked(QListWidgetItem *item)
    {
        auto sw = m_model->sw(QUrl(item->text()));

        auto form = new VMTDSwitchForm(nullptr, m_model, sw->identificator());

        form->show();
        form->raise();
        form->activateWindow();
    }
    void VMTDModelForm::pbAddSwitchClicked()
    {
        auto sw = new VMTDSwitch(nullptr, m_model->settings());

        do
        {
            sw->setIdentificator(VMTDRepo::generateIdentificator());
        }
        while (!m_model->addSwitch(sw));

        auto form = new VMTDSwitchForm(nullptr, m_model, sw->identificator());

        form->show();
        form->raise();
        form->activateWindow();

        updateSwitchesList();
    }
    void VMTDModelForm::pbRemoveSwitchClicked()
    {
        m_model->removeSwitch(m_model->sw(m_currentUrl)->identificator());

        updateSwitchesList();
    }

    void VMTDModelForm::lwNodesItemClicked(QListWidgetItem *item)
    {
        m_currentIp = item->text();
    }
    void VMTDModelForm::lwNodesItemDoubleClicked(QListWidgetItem *item)
    {
        auto node = m_model->node(item->text());

        auto form = new VMTDNodeForm(nullptr, m_model, node->identificator());

        form->show();
        form->raise();
        form->activateWindow();
    }
    void VMTDModelForm::pbAddNodeClicked()
    {
        auto node = new VMTDNode(nullptr, m_model->settings());

        do
        {
            node->setIdentificator(VMTDRepo::generateIdentificator());
        }
        while (!m_model->addNode(node));

        auto form = new VMTDNodeForm(nullptr, m_model, node->identificator());

        form->show();
        form->raise();
        form->activateWindow();

        updateNodesList();
    }
    void VMTDModelForm::pbRemoveNodeClicked()
    {
        m_model->removeNode(m_model->node(m_currentIp)->identificator());

        updateNodesList();
    }
}

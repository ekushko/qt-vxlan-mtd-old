#include    "VMTDEngineForm.h"
#include "ui_VMTDEngineForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDEngineForm::VMTDEngineForm(QWidget *parent, VMTDEngine *engine) :
        QWidget(parent),
        ui(new Ui::VMTDEngineForm),
        m_engine(engine),
        m_settings(engine->settings())
    {
        m_settings->creationOut(VN_S(VMTDEngineForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Engine");

        initializeView();
        updateView();

        m_settings->creationOut(VN_S(VMTDEngineForm) + " | Constructor finished");
    }

    VMTDEngineForm::~VMTDEngineForm()
    {
        m_settings->creationOut(VN_S(VMTDEngineForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDEngineForm) + " | Destructor finished");
    }

    void VMTDEngineForm::initializeView()
    {
        ui->trwGroups->setColumnHidden(1, true);

        connect(ui->trwGroups, &QTreeWidget::itemDoubleClicked,
                this, &VMTDEngineForm::trwGroupsItemDoubleClicked);
        connect(ui->pbMix, &QPushButton::clicked,
                m_engine, &VMTDEngine::mixSlot);
        connect(m_engine, &VMTDEngine::mixedSignal,
                this, &VMTDEngineForm::mixedSlot);
    }

    void VMTDEngineForm::updateView()
    {
        ui->trwGroups->clear();

        for (auto group : m_engine->groups())
        {
            auto groupItem = new QTreeWidgetItem(ui->trwGroups);
            groupItem->setText(0, QString("No. %1 VLAN ID: %2 ")
                               .arg(group->index())
                               .arg(group->vlanId()));


            auto participantItem = new QTreeWidgetItem(groupItem);
            participantItem->setText(0, "Participants");

            for (auto participant : m_engine->participants())
            {
                auto childItem = new QTreeWidgetItem(participantItem);
                childItem->setText(0, participant->nodeDevice()->name());
                childItem->setText(1, QString::number(participant->index_1()));
            }

            auto gatewayItem = new QTreeWidgetItem(groupItem);
            gatewayItem->setText(0, "Gateways");

            for (int i = 0; i < group->gateways().size(); ++i)
            {
                auto gateway = group->gateways().at(i);

                auto childItem = new QTreeWidgetItem(participantItem);
                childItem->setText(0, QString("%1 VLAN ID: %2")
                                   .arg(gateway->nodeDevice()->name())
                                   .arg(gateway->vlanId_1()));
                childItem->setText(1, QString::number(gateway->index_2()));
            }

            ui->trwGroups->addTopLevelItem(groupItem);
        }
    }

    void VMTDEngineForm::mixedSlot()
    {
        updateView();
    }

    void VMTDEngineForm::trwGroupsItemDoubleClicked(QTreeWidgetItem *item, int column)
    {
        Q_UNUSED(column)

        if (item->childCount() > 0)
            return;

        const auto index = item->text(1).toInt();

        if (item->text(0).contains(QString("VLAN")))
            m_engine->gateways().at(index)->showFormSlot();
        else
            m_engine->participants().at(index)->showFormSlot();
    }
}

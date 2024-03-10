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
        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDEngineForm::close);
    }

    void VMTDEngineForm::updateView()
    {
        ui->trwGroups->clear();

        for (auto group : m_engine->groups())
        {
            auto groupItem = new QTreeWidgetItem(ui->trwGroups);
            groupItem->setText(0, group->name());


            auto participantItem = new QTreeWidgetItem(groupItem);
            participantItem->setText(0, "Participants");

            for (auto participant : group->participants())
            {
                QString label = participant->vInterface1()->name();

//                if (participant->role() == VMTDParticipant::EnRole::GATEWAY)
//                    label += "  <--->  " + participant->vInterface2()->name();

                auto childItem = new QTreeWidgetItem(participantItem);
                childItem->setText(0, label);
                childItem->setText(1, QString::number(participant->vInterface1()->index()));
            }

            {
                auto gateway = group->gateway();

                if (gateway != nullptr)
                {
                    QString label = gateway->vInterface2()->name()
                                    /*+ "  <--->  " + gateway->vInterface1()->name()*/;

                    auto childItem = new QTreeWidgetItem(participantItem);
                    childItem->setText(0, label);
                    childItem->setText(1, QString::number(gateway->vInterface1()->index()));
                }
            }
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
        m_engine->participants().at(index)->showFormSlot();
    }
}

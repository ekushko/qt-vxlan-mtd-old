#include    "VMTDParticipantForm.h"
#include "ui_VMTDParticipantForm.h"

#include "../VMTDRepo.h"

namespace VMTDLib
{
    VMTDParticipantForm::VMTDParticipantForm(QWidget *parent, VMTDParticipant *participant) :
        QWidget(parent),
        ui(new Ui::VMTDParticipantForm),
        m_participant(participant),
        m_settings(m_participant->settings())
    {
        m_settings->creationOut(VN_S(VMTDParticipantForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(m_participant->nodeDevice()->name());

        initializeView();
        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDParticipantForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        m_settings->creationOut(VN_S(VMTDParticipantForm) + " | Constructor finished");
    }

    VMTDParticipantForm::~VMTDParticipantForm()
    {
        m_settings->creationOut(VN_S(VMTDParticipantForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDParticipantForm) + " | Destructor finished");
    }

    void VMTDParticipantForm::initializeView()
    {
        // do nothing
    }

    void VMTDParticipantForm::updateView()
    {
        ui->lbRole->setText(VMTDParticipant::enRoleToS(m_participant->role()));
        ui->lbNodeDevice->setText(m_participant->nodeDevice()->name());

        ui->lbGroupIndex_1->setText(QString::number(m_participant->groupIndex_1()));
        ui->lbIp_1->setText(m_participant->ip_1());
        ui->lbMask_1->setText(m_participant->mask_1());
        ui->lbVlanId_1->setText(QString::number(m_participant->vlanId_1()));

        ui->lbGroupIndex_2->setText(QString::number(m_participant->groupIndex_2()));
        ui->lbIp_2->setText(m_participant->ip_2());
        ui->lbMask_2->setText(m_participant->mask_2());
        ui->lbVlanId_2->setText(QString::number(m_participant->vlanId_2()));

        ui->lbGateway->setText(m_participant->gateway());

        qDeleteAll(m_routeForms);
        m_routeForms.clear();

        for (const auto &route : m_participant->routes())
        {
            auto lb = new QLabel(route, ui->wRoutes);
            m_routeForms.append(lb);

            ui->wRoutes->layout()->addWidget(lb);
        }
    }

    void VMTDParticipantForm::uiTimerTickSlot()
    {
        updateView();
    }
}

#include    "VMTDParticipantForm.h"
#include "ui_VMTDParticipantForm.h"

#include "../../VMTDRepo.h"

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
        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDParticipantForm::close);
    }

    void VMTDParticipantForm::updateView()
    {
        ui->lbRole->setText(VMTDParticipant::enRoleToS(m_participant->role()));
        ui->lbNodeDevice->setText(m_participant->nodeDevice()->name());

        ui->lbInterface_1->setText(m_participant->vInterface1()->toString());
        ui->lbInterface_2->setText(m_participant->vInterface2()->toString());

        bool isFirst;

        QString routesStr, hostsStr;

        isFirst = true;

        for (const auto &route : m_participant->vInterface1()->routes())
        {
            if (isFirst)
                isFirst = false;
            else
                routesStr += "\n";

            routesStr += route.toString();
        }


        isFirst = true;

        for (const auto &route : m_participant->vInterface2()->routes())
        {
            if (isFirst)
                isFirst = false;
            else
                routesStr += "\n";

            routesStr += route.toString();
        }


        isFirst = true;

        for (const auto &host : m_participant->hosts())
        {
            if (isFirst)
                isFirst = false;
            else
                hostsStr += "\n";

            hostsStr += host;
        }

        ui->lbRoutes->setText(routesStr.isEmpty() ? "Empty" : routesStr);
        ui->lbHosts->setText(hostsStr.isEmpty() ? "Empty" : hostsStr);
    }

    void VMTDParticipantForm::uiTimerTickSlot()
    {
        updateView();
    }
}

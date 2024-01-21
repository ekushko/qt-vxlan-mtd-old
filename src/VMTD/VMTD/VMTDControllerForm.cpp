#include    "VMTDControllerForm.h"
#include "ui_VMTDControllerForm.h"

#include "VMTDRepo.h"

namespace VMTDLib
{
    VMTDControllerForm::VMTDControllerForm(QWidget *parent, VMTDController *controller) :
        QWidget(parent),
        ui(new Ui::VMTDControllerForm),
        m_controller(controller)
    {
        m_controller->settings()->debugOut(VN_S(VMTDControllerForm) + " was created");

        ui->setupUi(this);

        setWindowTitle(m_controller->settings()->systemName());

        initializeView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDControllerForm::uiTimerTickSlot);
        m_uiTimer.start(100);
    }

    VMTDControllerForm::~VMTDControllerForm()
    {
        m_controller->settings()->debugOut(VN_S(VMTDControllerForm) + " was deleted");

        delete ui;
    }

    void VMTDControllerForm::initializeView()
    {
        connect(ui->pbSettings, &QPushButton::clicked,
                m_controller->settings(), &VMTDSettings::showFormSlot);

        connect(ui->pbStartController, &QPushButton::clicked,
                m_controller, &VMTDController::startController);
        connect(ui->pbStopController, &QPushButton::clicked,
                m_controller, &VMTDController::stopController);

        connect(ui->pbNxApiServer, &QPushButton::clicked,
                m_controller->nxApiServer(), &VMTDNxApiServer::showFormSlot);
        connect(ui->pbNodeServer, &QPushButton::clicked,
                m_controller->nodeServer(), &VMTDNodeServer::showFormSlot);

        connect(ui->pbNodeClient, &QPushButton::clicked,
                m_controller->nodeClient(), &VMTDNodeClient::showFormSlot);

        const auto nodeType = m_controller->settings()->nodeType();
        ui->pbNxApiServer->setVisible(nodeType == VMTDNodeType::SERVER);
        ui->pbNodeServer->setVisible(nodeType == VMTDNodeType::SERVER);
        ui->pbNodeClient->setVisible(nodeType == VMTDNodeType::CLIENT);
    }

    void VMTDControllerForm::uiTimerTickSlot()
    {
        const auto isRunning = m_controller->isRunning();

        ui->pbStartController->setEnabled(!isRunning);
        ui->pbStopController->setEnabled(isRunning);

        ui->pbNxApiServer->setEnabled(isRunning);
        ui->pbNodeServer->setEnabled(isRunning);
        ui->pbNodeClient->setEnabled(isRunning);
    }
}

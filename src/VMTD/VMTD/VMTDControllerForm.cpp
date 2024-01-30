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
        setAttribute(Qt::WA_DeleteOnClose, true);

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

        connect(ui->pbProtocol, &QPushButton::clicked,
                this, &VMTDControllerForm::pbProtocolClicked);

        connect(ui->pbNxApiServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNxApiServerClicked);
        connect(ui->pbNodeServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeServerClicked);
        connect(ui->pbNodeClient, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeClientClicked);

        const auto nodeType = m_controller->settings()->nodeType();
        ui->pbNxApiServer->setVisible(nodeType == VMTDNodeType::SERVER);
        ui->pbNodeServer->setVisible(nodeType == VMTDNodeType::SERVER);
        ui->pbNodeClient->setVisible(nodeType == VMTDNodeType::CLIENT);

        m_controller->net()->showFormSlot(ui->wMain);
    }

    void VMTDControllerForm::uiTimerTickSlot()
    {
        const auto isRunning = m_controller->isRunning();

        ui->pbStartController->setEnabled(!isRunning);
        ui->pbStopController->setEnabled(isRunning);

        ui->pbProtocol->setEnabled(isRunning);
        ui->pbNxApiServer->setEnabled(isRunning);
        ui->pbNodeServer->setEnabled(isRunning);
        ui->pbNodeClient->setEnabled(isRunning);
    }

    void VMTDControllerForm::pbProtocolClicked()
    {
        if (!m_controller->isRunning())
            return;

        m_controller->protocol()->showFormSlot();
    }

    void VMTDControllerForm::pbNxApiServerClicked()
    {
        if (!m_controller->isRunning())
            return;

        m_controller->nxApiServer()->showFormSlot();
    }

    void VMTDControllerForm::pbNodeServerClicked()
    {
        if (!m_controller->isRunning())
            return;

        m_controller->nodeServer()->showFormSlot();
    }

    void VMTDControllerForm::pbNodeClientClicked()
    {
        if (!m_controller->isRunning())
            return;

        m_controller->nodeClient()->showFormSlot();
    }
}

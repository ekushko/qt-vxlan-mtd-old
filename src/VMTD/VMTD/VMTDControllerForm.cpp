#include    "VMTDControllerForm.h"
#include "ui_VMTDControllerForm.h"

#include "VMTDBuildInfo.h"
#include "VMTDRepo.h"

namespace VMTDLib
{
    VMTDControllerForm::VMTDControllerForm(QWidget *parent, VMTDController *controller) :
        QWidget(parent),
        ui(new Ui::VMTDControllerForm),
        m_controller(controller),
        m_settings(controller->settings())
    {
        m_settings->creationOut(VN_S(VMTDControllerForm) + " | Constructor called");

        ui->setupUi(this);

        setWindowTitle("Menu");
        setAttribute(Qt::WA_DeleteOnClose, true);

        initializeView();

        connect(m_controller, &VMTDController::restartedSignal,
                this, &VMTDControllerForm::restartedSlot);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDControllerForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        m_settings->creationOut(VN_S(VMTDControllerForm) + " | Constructor finished");
    }

    VMTDControllerForm::~VMTDControllerForm()
    {
        m_settings->creationOut(VN_S(VMTDControllerForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDControllerForm) + " | Destructor finished");
    }

    void VMTDControllerForm::initializeView()
    {
        ui->wClient->setVisible(m_settings->nodeType() == VMTDNodeType::CLIENT);
        ui->wServer->setVisible(m_settings->nodeType() == VMTDNodeType::SERVER);

        connect(ui->pbDevices, &QPushButton::clicked,
                this, &VMTDControllerForm::pbDevicesClicked);
        connect(ui->pbConnections, &QPushButton::clicked,
                this, &VMTDControllerForm::pbConnectionsClicked);
        connect(ui->pbSettings_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbClientProtocol_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbClientProtocolClicked);
        connect(ui->pbNxApiServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNxApiServerClicked);
        connect(ui->pbNodeServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeServerClicked);
        connect(ui->pbNodeClient_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeClientClicked);
        connect(ui->pbExit_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbExitClicked);
        connect(ui->pbEngine, &QPushButton::clicked,
                this, &VMTDControllerForm::pbEngineClicked);
        connect(ui->pbConfigurator_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbConfiguratorClicked);

        connect(ui->pbServerProtocol, &QPushButton::clicked,
                this, &VMTDControllerForm::pbServerProtocolClicked);
        connect(ui->pbClientProtocol_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbClientProtocolClicked);
        connect(ui->pbConfigurator_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbConfiguratorClicked);
        connect(ui->pbNodeClient_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeClientClicked);
        connect(ui->pbSettings_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbExit_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbExitClicked);
    }

    void VMTDControllerForm::uiTimerTickSlot()
    {
        const auto isRunning = m_controller->isRunning();

        ui->pbServerProtocol->setEnabled(isRunning);
        ui->pbClientProtocol_1->setEnabled(isRunning);
        ui->pbClientProtocol_2->setEnabled(isRunning);
        ui->pbNxApiServer->setEnabled(isRunning);
        ui->pbNodeServer->setEnabled(isRunning);
        ui->pbNodeClient_1->setEnabled(isRunning);
        ui->pbNodeClient_2->setEnabled(isRunning);
    }

    void VMTDControllerForm::restartedSlot()
    {
        initializeView();
    }

    void VMTDControllerForm::pbDevicesClicked()
    {
        m_controller->deviceManager()->showFormSlot();
    }

    void VMTDControllerForm::pbConnectionsClicked()
    {
        m_controller->connectionManager()->showFormSlot();
    }

    void VMTDControllerForm::pbSettingsClicked()
    {
        m_controller->settings()->showFormSlot();
    }

    void VMTDControllerForm::pbServerProtocolClicked()
    {
        m_controller->protocol()->showFormSlot();
    }

    void VMTDControllerForm::pbClientProtocolClicked()
    {
        m_controller->protocol()->clientNodeHandler()->showFormSlot();
    }

    void VMTDControllerForm::pbEngineClicked()
    {
        m_controller->engine()->showFormSlot();
    }

    void VMTDControllerForm::pbConfiguratorClicked()
    {
        m_controller->configurator()->showFormSlot();
    }

    void VMTDControllerForm::pbNxApiServerClicked()
    {
        m_controller->nxApiServer()->showFormSlot();
    }

    void VMTDControllerForm::pbNodeServerClicked()
    {
        m_controller->nodeServer()->showFormSlot();
    }

    void VMTDControllerForm::pbNodeClientClicked()
    {
        m_controller->nodeClient()->showFormSlot();
    }

    void VMTDControllerForm::pbExitClicked()
    {
        if (!m_settings->isTrayMode())
            qApp->exit();
        else
            close();
    }
}

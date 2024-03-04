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
        if (m_settings->nodeType() == VMTDNodeType::SERVER)
            ui->wClient->hide();
        else
            ui->wServer->hide();

        connect(ui->pbDevices, &QPushButton::clicked,
                this, &VMTDControllerForm::pbDevicesClicked);
        connect(ui->pbConnections, &QPushButton::clicked,
                this, &VMTDControllerForm::pbConnectionsClicked);
        connect(ui->pbProtocol_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbProtocolClicked);
        connect(ui->pbEngine, &QPushButton::clicked,
                this, &VMTDControllerForm::pbEngineClicked);
        connect(ui->pbNxApiServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNxApiServerClicked);
        connect(ui->pbNodeServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeServerClicked);
        connect(ui->pbSettings_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbExit_2, &QPushButton::clicked,
                this, &VMTDControllerForm::pbExitClicked);

        connect(ui->pbProtocol_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbProtocolClicked);
        connect(ui->pbConfigurator, &QPushButton::clicked,
                this, &VMTDControllerForm::pbConfiguratorClicked);
        connect(ui->pbNodeClient, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeClientClicked);
        connect(ui->pbSettings_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbExit_1, &QPushButton::clicked,
                this, &VMTDControllerForm::pbExitClicked);
    }

    void VMTDControllerForm::uiTimerTickSlot()
    {
        const auto isRunning = m_controller->isRunning();

        ui->pbProtocol_1->setEnabled(isRunning);
        ui->pbProtocol_2->setEnabled(isRunning);
        ui->pbNxApiServer->setEnabled(isRunning);
        ui->pbNodeServer->setEnabled(isRunning);
        ui->pbNodeClient->setEnabled(isRunning);
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

    void VMTDControllerForm::pbProtocolClicked()
    {
        if (m_settings->nodeType() == VMTDNodeType::SERVER)
            m_controller->protocol()->showFormSlot();
        else
            m_controller->protocol()->handlers().at(0)->showFormSlot();
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
        qApp->exit();
    }
}

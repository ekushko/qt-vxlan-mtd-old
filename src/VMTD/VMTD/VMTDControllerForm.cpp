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

        setWindowTitle(m_settings->systemName());
        setAttribute(Qt::WA_DeleteOnClose, true);

        initializeView();

        tbwPartitionCurrentChangedSlot((int)EnTab::DEVICE_MANAGER);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDControllerForm::uiTimerTickSlot);
        m_uiTimer.start(100);

        uiTimerTickSlot();

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
        ui->lbVersion->setText(QString("Library %1: %2 from %3")
                               .arg(VMTDBuildInfo::filename())
                               .arg(VMTDBuildInfo::version())
                               .arg(VMTDBuildInfo::dateTime()
                                    .toString("dd-MM-yyyy hh:mm:ss")));

        connect(ui->pbSettings, &QPushButton::clicked,
                m_controller->settings(), &VMTDSettings::showFormSlot);

        connect(ui->pbStartController, &QPushButton::clicked,
                m_controller, &VMTDController::startController);
        connect(ui->pbStopController, &QPushButton::clicked,
                m_controller, &VMTDController::stopController);

        m_tabWidgets[EnTab::DEVICE_MANAGER] = ui->tbwPartition->widget((int)EnTab::DEVICE_MANAGER);
        m_tabWidgets[EnTab::CONN_MANAGER] = ui->tbwPartition->widget((int)EnTab::CONN_MANAGER);
        m_tabWidgets[EnTab::ENGINE] = ui->tbwPartition->widget((int)EnTab::ENGINE);
        m_tabWidgets[EnTab::PROTOCOL] = ui->tbwPartition->widget((int)EnTab::PROTOCOL);
        m_tabWidgets[EnTab::NODE_CLIENT] = ui->tbwPartition->widget((int)EnTab::NODE_CLIENT);
        m_tabWidgets[EnTab::NODE_SERVER] = ui->tbwPartition->widget((int)EnTab::NODE_SERVER);
        m_tabWidgets[EnTab::NXAPI_SERVER] = ui->tbwPartition->widget((int)EnTab::NXAPI_SERVER);

        connect(ui->tbwPartition, &QTabWidget::currentChanged,
                this, &VMTDControllerForm::tbwPartitionCurrentChangedSlot);
    }

    void VMTDControllerForm::uiTimerTickSlot()
    {
        const auto isRunning = m_controller->isRunning();

        ui->pbStartController->setEnabled(!isRunning);
        ui->pbStopController->setEnabled(isRunning);

        const auto nodeType = m_settings->nodeType();

        ui->tbwPartition->setTabEnabled((int)EnTab::ENGINE,
                                        isRunning);
        ui->tbwPartition->setTabEnabled((int)EnTab::PROTOCOL,
                                        isRunning);
        ui->tbwPartition->setTabEnabled((int)EnTab::NODE_CLIENT,
                                        isRunning && nodeType == VMTDNodeType::CLIENT);
        ui->tbwPartition->setTabEnabled((int)EnTab::NODE_SERVER,
                                        isRunning && nodeType == VMTDNodeType::SERVER);
        ui->tbwPartition->setTabEnabled((int)EnTab::NXAPI_SERVER,
                                        isRunning && nodeType == VMTDNodeType::SERVER);
    }

    void VMTDControllerForm::tbwPartitionCurrentChangedSlot(int index)
    {
        const auto isRunning = m_controller->isRunning();
        const auto nodeType = m_settings->nodeType();

        if (index == (int)EnTab::DEVICE_MANAGER)
        {
            m_controller->deviceManager()->showFormSlot(ui->wDevices);
        }
        else if (index == (int)EnTab::CONN_MANAGER)
        {
            m_controller->connectionManager()->showFormSlot(ui->wConnections);
        }
        else if (index == (int)EnTab::ENGINE)
        {
            m_controller->engine()->showFormSlot(ui->wEngine);
        }
        else if (index == (int)EnTab::PROTOCOL && isRunning)
        {
            if (nodeType == VMTDNodeType::SERVER)
                m_controller->protocol()->showFormSlot(ui->wProtocol);
            else if (nodeType == VMTDNodeType::CLIENT)
                m_controller->protocol()->handlers().at(0)->showFormSlot(ui->wProtocol);
        }
        else if (index == (int)EnTab::NODE_CLIENT && isRunning && nodeType == VMTDNodeType::CLIENT)
        {
            m_controller->nodeClient()->showFormSlot(ui->wNodeClient);
        }
        else if (index == (int)EnTab::NODE_SERVER && isRunning && nodeType == VMTDNodeType::SERVER)
        {
            m_controller->nodeServer()->showFormSlot(ui->wNodeServer);
        }
        else if (index == (int)EnTab::NXAPI_SERVER && isRunning && nodeType == VMTDNodeType::SERVER)
        {
            m_controller->nxApiServer()->showFormSlot(ui->wNxApiServer);
        }
    }
}

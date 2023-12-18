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

        setWindowTitle(QString("Контроллер %1").arg(m_controller->settings()->systemName()));

        initializeView();

        updateNxApiAdaptersList();
    }

    VMTDControllerForm::~VMTDControllerForm()
    {
        m_controller->settings()->debugOut(VN_S(VMTDControllerForm) + " was deleted");

        delete ui;
    }

    void VMTDControllerForm::initializeView()
    {
        connect(ui->pbSettings, &QPushButton::clicked,
                this, &VMTDControllerForm::pbSettingsClicked);
        connect(ui->pbStartController, &QPushButton::clicked,
                m_controller, &VMTDController::startController);
        connect(ui->pbStopController, &QPushButton::clicked,
                m_controller, &VMTDController::stopController);
        connect(ui->pbRefresh, &QPushButton::clicked,
                this, &VMTDControllerForm::pbRefreshClicked);
        connect(ui->pbNodeServer, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeServerClicked);
        connect(ui->pbNodeClient, &QPushButton::clicked,
                this, &VMTDControllerForm::pbNodeClientClicked);
        connect(ui->lwNxApiAdapters, &QListWidget::itemDoubleClicked,
                this, &VMTDControllerForm::lwNxApiAdaptersItemDoubleClicked);
    }

    void VMTDControllerForm::updateNxApiAdaptersList()
    {
        ui->lwNxApiAdapters->clear();

        for (auto nxApiAdapter : m_controller->nxApiAdapters())
            ui->lwNxApiAdapters->addItem(nxApiAdapter->url().toString());
    }

    void VMTDControllerForm::pbSettingsClicked()
    {
        m_controller->settings()->showForm();
    }

    void VMTDControllerForm::pbRefreshClicked()
    {
        updateNxApiAdaptersList();
    }

    void VMTDControllerForm::pbNodeServerClicked()
    {
        if (m_controller->settings()->nodeType() == VMTDNodeType::SERVER)
            m_controller->nodeServer()->showForm();
    }

    void VMTDControllerForm::pbNodeClientClicked()
    {
        if (m_controller->settings()->nodeType() == VMTDNodeType::CLIENT)
            m_controller->nodeClient()->showForm();
    }

    void VMTDControllerForm::lwNxApiAdaptersItemDoubleClicked(QListWidgetItem *item)
    {
        const auto row = ui->lwNxApiAdapters->row(item);
        m_controller->nxApiAdapters().at(row)->showForm();
    }
}

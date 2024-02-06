#include    "MainWindow.h"
#include "ui_MainWindow.h"

#include "VMTD/VMTDBuildInfo.h"
using namespace VMTDLib;

MainWindow::MainWindow(QWidget *parent, bool quickStart)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << QString("Library %1: %2 from %3")
             .arg(VMTDBuildInfo::filename())
             .arg(VMTDBuildInfo::version())
             .arg(VMTDBuildInfo::dateTime()
                  .toString("dd-MM-yyyy hh:mm:ss"));

    connect(ui->pbQuickStart, &QPushButton::clicked,
            this, &MainWindow::pbQuickStartClicked);

    connect(ui->pbCreate, &QPushButton::clicked,
            this, &MainWindow::pbCreateClicked);
    connect(ui->pbDelete, &QPushButton::clicked,
            this, &MainWindow::pbDeleteClicked);

    connect(ui->pbClose, &QPushButton::clicked,
            this, &MainWindow::close);

    m_uiTimer.setParent(this);
    connect(&m_uiTimer, &QTimer::timeout, this, &MainWindow::uiTimerTickSlot);
    m_uiTimer.start(100);

    if (quickStart)
        pbQuickStartClicked();
}

MainWindow::~MainWindow()
{
    delete m_controller;

    delete ui;
}

void MainWindow::uiTimerTickSlot()
{
    bool isCreated = m_controller != nullptr;
    bool isRunning = isCreated && m_controller->isRunning();

    ui->pbQuickStart->setEnabled(!isCreated);

    ui->pbCreate->setEnabled(!isCreated);
    ui->pbDelete->setEnabled(isCreated);

    ui->pbStart->setEnabled(isCreated && !isRunning);
    ui->pbStop->setEnabled(isRunning);

    ui->pbShowForm->setEnabled(isCreated);

    ui->pbSettings->setEnabled(isCreated);
}

void MainWindow::pbQuickStartClicked()
{
    if (m_controller != nullptr)
        return;

    pbCreateClicked();
    m_controller->startController();
    m_controller->showFormSlot();

    QTimer::singleShot(500, [this]()
    {
        close();
    });
}

void MainWindow::pbCreateClicked()
{
    if (m_controller != nullptr)
        return;

    m_controller = new VMTDController(this, m_systemName);
    connect(ui->pbStart, &QPushButton::clicked,
            m_controller.data(), &VMTDController::startController);
    connect(ui->pbStop, &QPushButton::clicked,
            m_controller.data(), &VMTDController::stopController);
    connect(ui->pbShowForm, &QPushButton::clicked,
            m_controller.data(), &VMTDController::showFormSlot);
    connect(ui->pbSettings, &QPushButton::clicked,
            m_controller->settings(), &VMTDSettings::showFormSlot);

}
void MainWindow::pbDeleteClicked()
{
    if (m_controller != nullptr)
        delete m_controller;
}


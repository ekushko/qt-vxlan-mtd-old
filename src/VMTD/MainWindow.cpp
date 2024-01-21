#include    "MainWindow.h"
#include "ui_MainWindow.h"

using namespace VMTDLib;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_settings = new VMTDSettings(this, VMTDNodeType::SERVER, "VMTD");
    connect(ui->pbSettings, &QPushButton::clicked, m_settings.data(), &VMTDSettings::showFormSlot);

    connect(ui->pbQuickStart, &QPushButton::clicked, this, &MainWindow::pbQuickStartClicked);

    connect(ui->pbCreate, &QPushButton::clicked, this, &MainWindow::pbCreateClicked);
    connect(ui->pbDelete, &QPushButton::clicked, this, &MainWindow::pbDeleteClicked);

    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pbQuickStartClicked()
{
    if (m_controller != nullptr)
        return;

    ui->pbCreate->click();
    ui->pbStart->click();
    ui->pbShowForm->click();

    close();
}

void MainWindow::pbCreateClicked()
{
    if (m_controller != nullptr)
        return;

    m_controller = new VMTDController(this, m_settings);
    connect(ui->pbStart, &QPushButton::clicked, m_controller.data(), &VMTDController::startController);
    connect(ui->pbStop, &QPushButton::clicked, m_controller.data(), &VMTDController::stopController);
    connect(ui->pbShowForm, &QPushButton::clicked, m_controller.data(), &VMTDController::showFormSlot);

}
void MainWindow::pbDeleteClicked()
{
    if (m_controller == nullptr)
        return;

    delete m_controller;
}


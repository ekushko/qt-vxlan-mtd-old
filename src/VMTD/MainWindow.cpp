#include    "MainWindow.h"
#include "ui_MainWindow.h"

using namespace VMTDLib;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbCreateServer, &QPushButton::clicked, this, &MainWindow::pbCreateServerClicked);
    connect(ui->pbCreateClient, &QPushButton::clicked, this, &MainWindow::pbCreateClientClicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pbCreateServerClicked()
{
    m_settings = new VMTDSettings(this, VMTDNodeType::SERVER, "VMTD_SERVER");

    m_server = new VMTDController(this, m_settings);
    m_server->startController();
    m_server->showForm();
}

void MainWindow::pbCreateClientClicked()
{
    m_settings = new VMTDSettings(this, VMTDNodeType::CLIENT, "VMTD_CLIENT");

    m_client = new VMTDController(this, m_settings);
    m_client->startController();
    m_client->showForm();
}


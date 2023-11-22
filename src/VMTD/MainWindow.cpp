#include    "MainWindow.h"
#include "ui_MainWindow.h"

using namespace VMTDLib;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbCreateServer, &QPushButton::clicked, this, &MainWindow::pbCreateServerClicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pbCreateServerClicked()
{
    m_settings = new VMTDSettings(this, "VMTD");
    m_controller = new VMTDController(this, m_settings);
    //m_controller->startController();

    m_controller->showForm();
}


#pragma once

#include "VMTD/VMTDController.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pbCreateServerClicked();

private:
    Ui::MainWindow *ui;

    VMTDLib::VMTDSettings *m_settings;
    VMTDLib::VMTDController *m_controller;
};

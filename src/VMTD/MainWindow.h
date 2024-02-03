#pragma once

#include "VMTD/VMTDController.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr, bool quickStart = false);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QTimer m_uiTimer;

    const QString m_systemName = "_vmtd";
    QPointer<VMTDLib::VMTDController> m_controller;

private slots:

    void uiTimerTickSlot();

    void pbQuickStartClicked();

    void pbCreateClicked();
    void pbDeleteClicked();
};

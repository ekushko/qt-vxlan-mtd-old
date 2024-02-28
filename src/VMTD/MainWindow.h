#pragma once

#include "VMTD/VMTDTray.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

enum class EnRunType
{
    STANDARD,
    QUICK_START,
    TRAY_MODE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr, EnRunType runType = EnRunType::STANDARD);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    QTimer m_uiTimer;

    const EnRunType m_runType;
    const QString m_systemName = "_vmtd";
    QPointer<VMTDLib::VMTDTray> m_tray;
    QPointer<VMTDLib::VMTDController> m_controller;

private slots:

    void uiTimerTickSlot();

    void pbQuickStartClicked();

    void pbCreateClicked();
    void pbDeleteClicked();
};

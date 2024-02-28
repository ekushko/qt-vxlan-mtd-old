#include "VMTDTray.h"

#include "VMTDRepo.h"

#include <QApplication>
#include <QAction>
#include <QMenu>

namespace VMTDLib
{
    VMTDTray::VMTDTray(QWidget *parent, VMTDController *controller)
        : QWidget(parent)
        , m_controller(controller)
        , m_settings(controller->settings())
    {
        m_settings->creationOut(VN_S(VMTDTray) + " | Constructor called");

        m_trayIcon = new QSystemTrayIcon(this);
        m_trayIcon->setIcon(QPixmap(":/favicon-tray.png"));

        m_trayIconMenu = new QMenu(this);

        auto showFormAction = new QAction("Menu", m_trayIconMenu);
        connect(showFormAction, &QAction::triggered, this, &VMTDTray::showControllerFormSlot);
        m_trayIconMenu->addAction(showFormAction);

        auto inspectModeAction = new QAction("Settings", m_trayIconMenu);
        connect(inspectModeAction, &QAction::triggered, this, &VMTDTray::showSettingsFormSlot);
        m_trayIconMenu->addAction(inspectModeAction);

        auto quitAction = new QAction("Exit", m_trayIconMenu);
        connect(quitAction, &QAction::triggered, this, &VMTDTray::exitSlot);
        m_trayIconMenu->addAction(quitAction);

        m_trayIcon->setContextMenu(m_trayIconMenu);

        m_settings->creationOut(VN_S(VMTDTray) + " | Constructor finished");
    }

    VMTDTray::~VMTDTray()
    {
        m_settings->creationOut(VN_S(VMTDTray) + " | Destructor called");

        // do nothing

        m_settings->creationOut(VN_S(VMTDTray) + " | Destructor finished");
    }

    void VMTDTray::show()
    {
        m_trayIcon->show();
    }
    void VMTDTray::hide()
    {
        m_trayIcon->hide();
    }

    void VMTDTray::showControllerFormSlot()
    {
        m_controller->showFormSlot();
    }

    void VMTDTray::showSettingsFormSlot()
    {
        m_settings->showFormSlot();
    }

    void VMTDTray::exitSlot()
    {
        qApp->exit();
    }
}

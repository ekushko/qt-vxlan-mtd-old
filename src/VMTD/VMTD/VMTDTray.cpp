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

        auto showMainFormAction = new QAction("Main", m_trayIconMenu);
        connect(showMainFormAction, &QAction::triggered, this, &VMTDTray::showMainFormSlot);
        m_trayIconMenu->addAction(showMainFormAction);

        auto settingsFormAction = new QAction("Settings", m_trayIconMenu);
        connect(settingsFormAction, &QAction::triggered, this, &VMTDTray::showSettingsFormSlot);
        m_trayIconMenu->addAction(settingsFormAction);

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

    void VMTDTray::showMainFormSlot()
    {
        m_controller->showMainFormSlot();
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

#pragma once

#include "VMTDController.h"

#include <QWidget>
#include <QSystemTrayIcon>

namespace VMTDLib
{
    class VMTDTray : public QWidget
    {
        Q_OBJECT
        Q_DISABLE_COPY(VMTDTray)

    public:

        VMTDTray(QWidget *parent, VMTDController *controller);
        ~VMTDTray();

        void show();
        void hide();

    private:

        VMTDController *m_controller;
        VMTDSettings *m_settings;

        QSystemTrayIcon *m_trayIcon;
        QMenu *m_trayIconMenu;

    private slots:

        void showControllerFormSlot();
        void showSettingsFormSlot();
        void exitSlot();
    };
}

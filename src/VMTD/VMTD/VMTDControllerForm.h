#pragma once

#include "VMTDController.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDControllerForm;
}

namespace VMTDLib
{
    class VMTDControllerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDControllerForm(QWidget *parent, VMTDController *controller);
        ~VMTDControllerForm();

    private:

        void initializeView();

        Ui::VMTDControllerForm *ui;

        VMTDController *m_controller;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void restartedSlot();

        void pbDevicesClicked();
        void pbConnectionsClicked();
        void pbSettingsClicked();
        void pbProtocolClicked();
        void pbEngineClicked();
        void pbConfiguratorClicked();
        void pbNxApiServerClicked();
        void pbNodeServerClicked();
        void pbNodeClientClicked();
        void pbExitClicked();
    };
}

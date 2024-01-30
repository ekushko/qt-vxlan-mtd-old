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

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void pbProtocolClicked();

        void pbNxApiServerClicked();
        void pbNodeServerClicked();
        void pbNodeClientClicked();
    };
}

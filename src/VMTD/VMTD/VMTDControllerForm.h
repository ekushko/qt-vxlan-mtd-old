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

        enum class EnTab
        {
            NET          = 0,
            PROTOCOL     = 1,
            NODE_CLIENT  = 2,
            NODE_SERVER  = 3,
            NXAPI_SERVER = 4
        };

        QMap<EnTab, QWidget *> m_tabWidgets;

    private slots:

        void uiTimerTickSlot();

        void tbwPartitionCurrentChangedSlot(int index);
    };
}

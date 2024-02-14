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
            DEVICE_MANAGER = 0,
            CONN_MANAGER   = 1,
            ENGINE         = 2,
            PROTOCOL       = 3,
            NODE_CLIENT    = 4,
            NODE_SERVER    = 5,
            NXAPI_SERVER   = 6
        };

        QMap<EnTab, QWidget *> m_tabWidgets;

    private slots:

        void uiTimerTickSlot();

        void tbwPartitionCurrentChangedSlot(int index);
    };
}

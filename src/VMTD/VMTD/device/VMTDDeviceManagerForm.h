#pragma once

#include "VMTDDeviceManager.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDDeviceManagerForm;
}

namespace VMTDLib
{
    class VMTDDeviceManagerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDDeviceManagerForm(QWidget *parent, VMTDDeviceManager *net);
        ~VMTDDeviceManagerForm();

    private:

        void updateNxApiDevicesList();
        void updateNodeDevicesList();

        Ui::VMTDDeviceManagerForm *ui;

        VMTDDeviceManager *m_net;
        VMTDSettings *m_settings;

        int m_currentNxApiDeviceId;
        int m_currentNodeDeviceId;

    private slots:

        void lwNxApiDevicesItemClicked(QListWidgetItem *item);
        void lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item);
        void pbAddNxApiDeviceClicked();
        void pbRemoveNxApiDeviceClicked();

        void lwNodeDevicesItemClicked(QListWidgetItem *item);
        void lwNodeDevicesItemDoubleClicked(QListWidgetItem *item);
        void pbAddNodeDeviceClicked();
        void pbRemoveNodeDeviceClicked();
    };
}

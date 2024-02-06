#pragma once

#include "VMTDNet.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDNetForm;
}

namespace VMTDLib
{
    class VMTDNetForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNetForm(QWidget *parent, VMTDNet *net);
        ~VMTDNetForm();

    private:

        void updateNxApiDevicesList();
        void updateNodeDevicesList();

        Ui::VMTDNetForm *ui;

        VMTDNet *m_net;

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

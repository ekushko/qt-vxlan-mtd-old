#pragma once

#include "VMTDNxApiDevice.h"

#include <QWidget>

namespace Ui
{
    class VMTDNxApiDeviceForm;
}

namespace VMTDLib
{
    class VMTDNxApiDeviceForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiDeviceForm(QWidget *parent, VMTDNxApiDevice *device);
        ~VMTDNxApiDeviceForm();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

    private:

        void initializeView();

        Ui::VMTDNxApiDeviceForm *ui;

        VMTDNxApiDevice *m_device;
        VMTDSettings *m_settings;

    private slots:

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

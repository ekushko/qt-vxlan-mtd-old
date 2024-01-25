#pragma once

#include "VMTDNodeDevice.h"

#include <QWidget>

namespace Ui
{
    class VMTDNodeDeviceForm;
}

namespace VMTDLib
{
    class VMTDNodeDeviceForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNodeDeviceForm(QWidget *parent, VMTDNodeDevice *device);
        ~VMTDNodeDeviceForm();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

    private:

        void initializeView();

        Ui::VMTDNodeDeviceForm *ui;

        VMTDNodeDevice *m_device;

    private slots:

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

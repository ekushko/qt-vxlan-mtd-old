#pragma once

#include "../device/VMTDDevice.h"
#include "../interface/VMTDInterface.h"

#include <QWidget>

namespace Ui
{
    class VMTDConnectionForm;
}

namespace VMTDLib
{
    class VMTDConnectionForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDConnectionForm(QWidget *parent,
                           VMTDDevice *device_1, VMTDInterface *interface_1,
                           VMTDDevice *device_2, VMTDInterface *interface_2);
        ~VMTDConnectionForm();

        VMTDDevice *device_1() const;
        VMTDInterface *interface_1() const;

        VMTDDevice *device_2() const;
        VMTDInterface *interface_2() const;

    signals:

        void removeSignal();

    private:

        void initializeView();

        Ui::VMTDConnectionForm *ui;

        VMTDSettings *m_settings;

        VMTDDevice *m_device_1;
        VMTDInterface *m_interface_1;

        VMTDDevice *m_device_2;
        VMTDInterface *m_interface_2;
    };
}

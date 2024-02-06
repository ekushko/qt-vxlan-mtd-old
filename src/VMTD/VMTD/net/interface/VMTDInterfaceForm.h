#pragma once

#include "VMTDInterface.h"

#include <QWidget>

namespace Ui
{
    class VMTDInterfaceForm;
}

namespace VMTDLib
{
    class VMTDInterfaceForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDInterfaceForm(QWidget *parent, VMTDInterface *interface);
        ~VMTDInterfaceForm();

        int id() const;

        void updateView();

        void updateData();

    private:

        Ui::VMTDInterfaceForm *ui;

        VMTDInterface *m_interface;
        VMTDSettings *m_settings;
    };
}

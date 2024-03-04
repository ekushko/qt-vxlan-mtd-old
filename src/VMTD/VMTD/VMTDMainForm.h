#pragma once

#include "VMTDController.h"

#include <QWidget>

namespace Ui
{
    class VMTDMainForm;
}

namespace VMTDLib
{
    class VMTDMainForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDMainForm(QWidget *parent, VMTDController *controller);
        ~VMTDMainForm();

    private:
        Ui::VMTDMainForm *ui;

        VMTDController *m_controller;
    };
}

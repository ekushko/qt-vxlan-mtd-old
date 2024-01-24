#pragma once

#include "VMTDInterfaces.h"
#include "VMTDInterfaceForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDInterfacesForm;
}

namespace VMTDLib
{
    class VMTDInterfacesForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDInterfacesForm(QWidget *parent, VMTDInterfaces *interfaces);
        ~VMTDInterfacesForm();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

    private:

        Ui::VMTDInterfacesForm *ui;

        VMTDInterfaces *m_interfaces;

        QVector<VMTDInterfaceForm *> m_interfaceForms;

    private slots:

        void interfaceCreatedSlot(int id);
        void interfaceRemovedSlot(int id);

        void pbAddClicked();
        void pbRemoveClicked();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

#pragma once

#include "VMTDInterfaceManager.h"
#include "VMTDInterfaceForm.h"

#include <QWidget>

namespace Ui
{
    class VMTDInterfaceManagerForm;
}

namespace VMTDLib
{
    class VMTDInterfaceManagerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDInterfaceManagerForm(QWidget *parent, VMTDInterfaceManager *manager);
        ~VMTDInterfaceManagerForm();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

    private:

        void initializeView();

        Ui::VMTDInterfaceManagerForm *ui;

        VMTDInterfaceManager *m_manager;
        VMTDSettings *m_settings;

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

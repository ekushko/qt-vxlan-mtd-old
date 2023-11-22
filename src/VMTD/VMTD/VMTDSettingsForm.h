#pragma once

#include "VMTDSettings.h"

#include <QWidget>

namespace Ui
{
    class VMTDSettingsForm;
}

namespace VMTDLib
{
    class VMTDSettingsForm : public QWidget
    {
        Q_OBJECT

    public:
        VMTDSettingsForm(QWidget *parent, VMTDSettings *settings);
        ~VMTDSettingsForm();

    private slots:
        void pbSaveClicked();
        void pbLoadClicked();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();

    private:
        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

        Ui::VMTDSettingsForm *ui;

        VMTDSettings *m_settings;
    };
}

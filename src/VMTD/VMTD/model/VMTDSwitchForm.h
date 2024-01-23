#pragma once

#include "VMTDSwitchPortForm.h"

#include <QTimer>

namespace Ui
{
    class VMTDSwitchForm;
}

namespace VMTDLib
{
    class VMTDSwitchForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDSwitchForm(QWidget *parent,
                       VMTDModel *model,
                       int identificator);
        ~VMTDSwitchForm();

    private:

        void initializeView();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

        Ui::VMTDSwitchForm *ui;

        VMTDModel *m_model;
        VMTDSwitch *m_sw;

        int m_identificator;

        bool m_isEditMode;

        QTimer m_uiTimer;

        VMTDSwitchPortForm *m_portForm;

    private slots:

        void portCountChangedSlot();

        void uiTimerTickSlot();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

#pragma once

#include "VMTDModel.h"

#include <QWidget>
#include <QTimer>

namespace Ui
{
    class VMTDNodeForm;
}

namespace VMTDLib
{
    class VMTDNodeForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNodeForm(QWidget *parent,
                     VMTDModel *model,
                     int identificator);
        ~VMTDNodeForm();

    private:

        void initializeView();

        void setEditMode(bool isEditMode);
        void updateView();

        void updateData();

        Ui::VMTDNodeForm *ui;

        VMTDModel *m_model;
        VMTDNode *m_node;

        int m_identificator;

        bool m_isEditMode;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void cbCurrentSwitchIndexChanged(int index);

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

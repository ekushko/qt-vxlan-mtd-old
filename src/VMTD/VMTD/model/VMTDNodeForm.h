#pragma once

#include "VMTDModel.h"

#include <QWidget>

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

    private slots:

        void cbCurrentSwitchIndexChanged(int index);

        void pbRefreshClicked();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

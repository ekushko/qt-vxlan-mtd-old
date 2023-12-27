#pragma once

#include "VMTDModel.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

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

        void updateViewPorts(int portCount);

        Ui::VMTDSwitchForm *ui;

        VMTDModel *m_model;
        VMTDSwitch *m_sw;

        int m_identificator;

        bool m_isEditMode;

        QVector<QWidget *> m_wPorts;
        QVector<QLineEdit *> m_lePortInterfaces;
        QVector<QComboBox *> m_cbPortNodes;

    private slots:

        void sbPortCountValueChanged(int value);

        void pbRefreshClicked();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();
    };
}

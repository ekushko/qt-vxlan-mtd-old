#pragma once

#include "VMTDModel.h"

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

namespace Ui
{
    class VMTDSwitchPortForm;
}

namespace VMTDLib
{
    class VMTDSwitchPortForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDSwitchPortForm(QWidget *parent,
                           VMTDModel *model,
                           int id);
        ~VMTDSwitchPortForm();

        void reinitializeView();
        void updateView();

        void updateData();

    private:

        void initializeView();

        Ui::VMTDSwitchPortForm *ui;

        VMTDModel *m_model;
        VMTDSwitch *m_sw;

        int m_id;

        QVector<QWidget *> m_wPorts;
        QVector<QLineEdit *> m_lePortInterfaces;
        QVector<QComboBox *> m_cbPortNodes;
    };
}

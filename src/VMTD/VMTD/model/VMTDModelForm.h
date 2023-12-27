#pragma once

#include "VMTDModel.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDModelForm;
}

namespace VMTDLib
{
    class VMTDModelForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDModelForm(QWidget *parent, VMTDModel *model);
        ~VMTDModelForm();

    private:

        void updateSwitchesList();
        void updateNodesList();

        Ui::VMTDModelForm *ui;

        VMTDModel *m_model;

        QUrl m_currentUrl;
        QString m_currentIp;

    private slots:

        void lwSwitchesItemClicked(QListWidgetItem *item);
        void lwSwitchesItemDoubleClicked(QListWidgetItem *item);
        void pbAddSwitchClicked();
        void pbRemoveSwitchClicked();

        void lwNodesItemClicked(QListWidgetItem *item);
        void lwNodesItemDoubleClicked(QListWidgetItem *item);
        void pbAddNodeClicked();
        void pbRemoveNodeClicked();
    };
}

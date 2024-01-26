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

        void updateNxApiDevicesList();
        void updateNodeDevicesList();

        Ui::VMTDModelForm *ui;

        VMTDModel *m_model;

        QUrl m_currentUrl;
        QString m_currentIp;

    private slots:

        void lwNxApiDevicesItemClicked(QListWidgetItem *item);
        void lwNxApiDevicesItemDoubleClicked(QListWidgetItem *item);
        void pbAddNxApiDeviceClicked();
        void pbRemoveNxApiDeviceClicked();

        void lwNodeDevicesItemClicked(QListWidgetItem *item);
        void lwNodeDevicesItemDoubleClicked(QListWidgetItem *item);
        void pbAddNodeDeviceClicked();
        void pbRemoveNodeDeviceClicked();
    };
}

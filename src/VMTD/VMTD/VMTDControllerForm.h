#pragma once

#include "VMTDController.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui
{
    class VMTDControllerForm;
}

namespace VMTDLib
{
    class VMTDControllerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDControllerForm(QWidget *parent, VMTDController *controller);
        ~VMTDControllerForm();

    private:

        void initialize();

        void updateNxApiAdaptersList();

        Ui::VMTDControllerForm *ui;

        VMTDController *m_controller;

    private slots:

        void pbSettingsClicked();

        void pbRefreshClicked();

        void pbSaveClicked();
        void pbLoadClicked();

        void pbCreateNxApiAdapterClicked();
        void pbDeleteNxApiAdapterClicked();

        void lwNxApiAdaptersItemDoubleClicked(QListWidgetItem *item);
    };
}

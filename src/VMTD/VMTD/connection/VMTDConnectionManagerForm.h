#pragma once

#include "VMTDConnectionManager.h"
#include "VMTDConnectionForm.h"

#include <QWidget>
#include <QComboBox>

namespace Ui
{
    class VMTDConnectionManagerForm;
}

namespace VMTDLib
{
    class VMTDConnectionManagerForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDConnectionManagerForm(QWidget *parent, VMTDConnectionManager *manager);
        ~VMTDConnectionManagerForm();

    private:

        void   initializeView();
        void reinitializeView();

        void fillDevices(QComboBox *cb);
        void fillInterfaces(QComboBox *cb, VMTDDevice *device);
        void fillConnections(QWidget *w, VMTDDevice *device);

        Ui::VMTDConnectionManagerForm *ui;

        VMTDConnectionManager *m_manager;
        VMTDSettings *m_settings;

        QVector<VMTDConnectionForm *> m_connForms;

    private slots:

        void removeSlot();

        void pbReloadClicked();
        void pbCreateClicked();

        void cbDeviceCurrentIndexChanged(int index);
        void cbDeviceCurrentIndexChanged_1(int index);
        void cbDeviceCurrentIndexChanged_2(int index);
    };
}

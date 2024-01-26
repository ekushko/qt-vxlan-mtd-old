#pragma once

#include <QWidget>
#include <QTimer>

#include "VMTDNxApiAdapter.h"

namespace Ui
{
    class VMTDNxApiAdapterForm;
}

namespace VMTDLib
{
    class VMTDNxApiAdapterForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNxApiAdapterForm(QWidget *parent, VMTDNxApiAdapter *adapter);
        ~VMTDNxApiAdapterForm();

        void updateView();

    signals:

        void checkConnectionSignal();

        void sendCommandsSignal(const QStringList &commands);

    public slots:

        void showMessageSlot(const QString &text);

    private:

        void initializeView();

        Ui::VMTDNxApiAdapterForm *ui;

        VMTDNxApiAdapter *m_adapter;


    private slots:

        void pbCheckConnectionClicked();

        void pbSendCommandsClicked();
    };
}

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

        void sendCommandSignal(const QStringList &commands);

    public slots:

        void showDebugSlot(const QTime &time, const QString &text);

    private:

        void initializeView();

        Ui::VMTDNxApiAdapterForm *ui;

        VMTDNxApiAdapter *m_adapter;


    private slots:

        void pbSendCommandClicked();

        void pbClearFlowClicked();
    };
}

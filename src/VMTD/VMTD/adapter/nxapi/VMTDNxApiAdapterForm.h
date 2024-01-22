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

    signals:

        void checkConnectionSignal();

        void sendCommandsSignal(const QStringList &commands);

    public slots:

        void showMessageSlot(const QString &text);

    private:

        void initializeView();

        Ui::VMTDNxApiAdapterForm *ui;

        VMTDNxApiAdapter *m_adapter;

        QTimer m_uiTimer;

    private slots:

        void uiTimerTickSlot();

        void pbCheckConnectionClicked();

        void pbSendCommandsClicked();
    };
}

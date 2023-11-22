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

    private slots:
        void      uiTimerTickSlot();
        void quickUiTimerTickSlot();

        void pbHideRightClicked();

        void pbCheckConnectionClicked();

        void pbSendCommandsClicked();

        void pbChangeClicked();
        void pbAcceptClicked();
        void pbCancelClicked();

    private:
        void initialize();

        void setEditMode(bool isEditMode);

        Ui::VMTDNxApiAdapterForm *ui;

        VMTDNxApiAdapter *m_adapter;

        QTimer m_uiTimer;
        QTimer m_quickUiTimer;
    };
}

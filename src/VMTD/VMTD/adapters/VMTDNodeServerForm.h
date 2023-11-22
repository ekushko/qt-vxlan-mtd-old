#pragma once

#include "VMTDNodeServer.h"

#include "VMTDNodeAdapterForm.h"

#include <QWidget>
#include <QPlainTextEdit>

namespace Ui
{
    class VMTDNodeServerForm;
}

namespace VMTDLib
{
    class VMTDNodeServerForm : public QWidget
    {
        Q_OBJECT

    public:
        VMTDNodeServerForm(QWidget *parent, VMTDNodeServer *server);
        ~VMTDNodeServerForm();

    public slots:
        void showDebugSlot(QWebSocket *socket, const QString &debugString);

    private slots:
        void uiTimerTickSlot();

        void appendClientSocketSlot(QWebSocket *socket);
        void removeClientSocketSlot(QWebSocket *socket);

        void pbShowDetailedStateClicked();

        void pbClearErrorsClicked();

    private:
        void initializeView();
        void updateView();

        Ui::VMTDNodeServerForm *ui;

        VMTDNodeServer *m_server;

        QTimer m_uiTimer;

        QMap<QWebSocket *, VMTDNodeAdapterForm *> m_socketToForm;
    };
}

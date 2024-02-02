#pragma once

#include "../VMTDAdapterForm.h"

#include <QWidget>
#include <QWebSocket>

namespace Ui
{
    class VMTDNodeAdapterForm;
}

namespace VMTDLib
{
    class VMTDNodeAdapterForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDNodeAdapterForm(QWidget *parent, QWebSocket *socket);
        ~VMTDNodeAdapterForm();

    public slots:

        void showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text);

    private:

        Ui::VMTDNodeAdapterForm *ui;

        VMTDAdapterForm *m_adapterForm;

        QWebSocket *m_socket;
    };
}

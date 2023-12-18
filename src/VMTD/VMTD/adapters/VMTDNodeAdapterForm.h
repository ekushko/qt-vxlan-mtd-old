#pragma once

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

    signals:

        void sendMessageSignal(QWebSocket *socket, const QJsonObject &messageObj);

    public slots:

        void showDebugSlot(QWebSocket *socket, const QString &text);

    private:

        Ui::VMTDNodeAdapterForm *ui;

        QWebSocket *m_socket;

    private slots:

        void pbSendClicked();
    };
}

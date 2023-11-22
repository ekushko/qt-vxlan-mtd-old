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
        void sendNodeParamsSignal(QWebSocket *socket, const QJsonObject &nodeParamsObj);

    public slots:
        void showDebugSlot(QWebSocket *socket, const QString &debugString);

    private slots:
        void pbSendClicked();

    private:
        Ui::VMTDNodeAdapterForm *ui;

        QWebSocket *m_socket;
    };
}

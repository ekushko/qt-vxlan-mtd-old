#include    "VMTDNodeAdapterForm.h"
#include "ui_VMTDNodeAdapterForm.h"

#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNodeAdapterForm::VMTDNodeAdapterForm(QWidget *parent, QWebSocket *socket) :
        QWidget(parent),
        ui(new Ui::VMTDNodeAdapterForm),
        m_socket(socket)
    {
        ui->setupUi(this);

        connect(ui->pbSend, &QPushButton::clicked,
                this, &VMTDNodeAdapterForm::pbSendClicked);
    }

    VMTDNodeAdapterForm::~VMTDNodeAdapterForm()
    {
        delete ui;
    }

    void VMTDNodeAdapterForm::showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text)
    {
        if (m_socket == socket)
        {
            ui->pteFlow->appendPlainText(QString("\n[%1] %2\n")
                                         .arg(time.toString("hh:mm:ss:zzz"))
                                         .arg(text));
        }
    }

    void VMTDNodeAdapterForm::pbSendClicked()
    {
        const auto text = ui->pteMessage->toPlainText();
        const auto jsonDoc = QJsonDocument::fromJson(text.toUtf8());

        if (jsonDoc.isNull() || !jsonDoc.isObject())
        {
            showDebugSlot(m_socket, QTime::currentTime(),
                          "Message not sended! This is not JSON!");
            return;
        }

        emit sendMessageSignal(m_socket, jsonDoc.object());

        ui->pteMessage->clear();
    }
}

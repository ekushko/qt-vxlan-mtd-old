#include    "VMTDNodeAdapterForm.h"
#include "ui_VMTDNodeAdapterForm.h"

#include <QJsonValue>
#include <QJsonObject>

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

    void VMTDNodeAdapterForm::showDebugSlot(QWebSocket *socket, const QString &text)
    {
        if (m_socket == socket)
        {
            ui->pteFlow->appendPlainText("\n--------------------------------\n");
            ui->pteFlow->appendPlainText(text);
            ui->pteFlow->appendPlainText("\n--------------------------------\n");
        }
    }

    void VMTDNodeAdapterForm::pbSendClicked()
    {
        QJsonValue jsonValue(ui->pteMessage->toPlainText());

        emit sendMessageSignal(m_socket, jsonValue.toObject());

        ui->pteMessage->clear();
    }
}

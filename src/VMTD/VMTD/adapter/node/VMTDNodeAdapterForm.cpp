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

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        m_adapterForm = new VMTDAdapterForm(ui->wAdapter);
    }

    VMTDNodeAdapterForm::~VMTDNodeAdapterForm()
    {
        delete ui;
    }

    void VMTDNodeAdapterForm::showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text)
    {
        if (m_socket == socket)
        {
            m_adapterForm->appendText(QString("\n[%1] %2\n")
                                      .arg(time.toString("hh:mm:ss:zzz"))
                                      .arg(text));
        }
    }
}

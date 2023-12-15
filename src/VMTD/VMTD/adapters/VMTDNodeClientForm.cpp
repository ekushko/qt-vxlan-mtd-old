#include    "VMTDNodeClientForm.h"
#include "ui_VMTDNodeClientForm.h"

namespace VMTDLib
{
    VMTDNodeClientForm::VMTDNodeClientForm(QWidget *parent, VMTDNodeClient *client) :
        QWidget(parent),
        ui(new Ui::VMTDNodeClientForm),
        m_client(client)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);

        m_adapterForm = new VMTDNodeAdapterForm(ui->wLeft, m_client->socket());
        m_adapterForm->layout()->setMargin(0);
        connect(m_adapterForm, &VMTDNodeAdapterForm::sendRequestSignal,
                this, &VMTDNodeClientForm::sendRequestSignal);
        connect(m_client, &VMTDNodeClient::showDebugSignal,
                m_adapterForm, &VMTDNodeAdapterForm::showDebugSlot);
        ui->wLeft->layout()->addWidget(m_adapterForm);

        initializeView();

        connect(ui->pbDisconnectSocket, &QPushButton::clicked,
                m_client, &VMTDNodeClient::disconnectSocketSlot);
        connect(ui->pbConnectSocket, &QPushButton::clicked,
                m_client, &VMTDNodeClient::connectSocketSlot);

        connect(this, &VMTDNodeClientForm::sendRequestSignal,
                m_client, &VMTDNodeClient::sendRequestSlot);

        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeClientForm::uiTimerTickSlot);
        m_uiTimer.start(500);
    }

    VMTDNodeClientForm::~VMTDNodeClientForm()
    {
        delete ui;
    }

    void VMTDNodeClientForm::initializeView()
    {
        // do nothing
    }

    void VMTDNodeClientForm::updateView()
    {
        if (m_client == nullptr)
        {
            ui->lbSocketState->setText("Состояние сокета: н/д");
            ui->lbOpen->setText("Сокет открыт: Нет");
            ui->lbLocalAdress->setText("Локальный адрес: н/д");
            ui->lbPeerAdress->setText("Удаленный адрес: н/д");

            setWindowTitle("WebSocket-клиент: н/д:н/д");

            return;
        }

        const QString title = QString("WebSocket-клиент: %1:%2")
                              .arg(m_client->settings()->serverIp())
                              .arg(m_client->settings()->serverPort());
        setWindowTitle(title);

        const auto socketState = m_client->socket()->state();

        ui->lbSocketState->setText("Состояние сокета: "
                                   + QVariant::fromValue(socketState).toString());

        if (socketState == QAbstractSocket::ConnectedState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(true);
            ui->lbOpen->setText("Сокет открыт: Да");
        }
        else if (socketState == QAbstractSocket::ConnectingState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Сокет открыт: Нет");
        }
        else
        {
            ui->pbConnectSocket->setEnabled(true);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Сокет открыт: Нет");
        }

        QString ipAdress;

        if (!m_client->socket()->localAddress().isNull())
        {
            ipAdress = QString("Локальный адрес: %1:%2")
                       .arg(m_client->socket()->localAddress().toString())
                       .arg(m_client->socket()->localPort());
            ui->lbLocalAdress->setText(ipAdress);
        }
        else
        {
            ui->lbLocalAdress->setText("Локальный адрес: н/д");
        }

        if (!m_client->socket()->peerAddress().isNull())
        {
            ipAdress = QString("Удаленный адрес: %1:%2")
                       .arg(m_client->socket()->peerAddress().toString())
                       .arg(m_client->socket()->peerPort());
            ui->lbPeerAdress->setText(ipAdress);
        }
        else
        {
            ui->lbPeerAdress->setText("Удаленный адрес: н/д");
        }

        if (ui->pteSocketErrors->toPlainText() != m_client->socketErrors())
        {
            ui->pteSocketErrors->clear();
            ui->pteSocketErrors->appendPlainText(m_client->socketErrors());
        }
    }

    void VMTDNodeClientForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDNodeClientForm::pbShowDetailedStateClicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? "<" : ">");
    }

    void VMTDNodeClientForm::pbClearErrorsClicked()
    {
        m_client->clearSocketErrors();
    }

}

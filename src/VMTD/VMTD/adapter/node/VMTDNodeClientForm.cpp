#include    "VMTDNodeClientForm.h"
#include "ui_VMTDNodeClientForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeClientForm::VMTDNodeClientForm(QWidget *parent, VMTDNodeClient *client) :
        QWidget(parent),
        ui(new Ui::VMTDNodeClientForm),
        m_client(client),
        m_settings(client->settings())
    {
        m_settings->creationOut(VN_S(VMTDNodeClientForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        if (parent != nullptr)
            ui->pbClose->hide();

        setAttribute(Qt::WA_DeleteOnClose, true);

        m_adapterForm = new VMTDNodeAdapterForm(ui->wLeft, m_client->socket());
        m_adapterForm->layout()->setMargin(0);
        connect(m_client, &VMTDNodeClient::showDebugSignal,
                m_adapterForm, &VMTDNodeAdapterForm::showDebugSlot);
        ui->wLeft->layout()->addWidget(m_adapterForm);

        initializeView();

        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeClientForm::uiTimerTickSlot);
        m_uiTimer.start(500);

        m_settings->creationOut(VN_S(VMTDNodeClientForm) + " | Constructor finished");
    }

    VMTDNodeClientForm::~VMTDNodeClientForm()
    {
        m_settings->creationOut(VN_S(VMTDNodeClientForm) + " | Destructor called");

        delete ui;

        m_settings->creationOut(VN_S(VMTDNodeClientForm) + " | Destructor finished");
    }

    void VMTDNodeClientForm::initializeView()
    {
        connect(ui->pbDisconnectSocket, &QPushButton::clicked,
                m_client, &VMTDNodeClient::disconnectSocketSlot);
        connect(ui->pbConnectSocket, &QPushButton::clicked,
                m_client, &VMTDNodeClient::connectSocketSlot);

        connect(ui->pbShowDetailedState, &QPushButton::clicked,
                this, &VMTDNodeClientForm::pbShowDetailedStateClicked);

        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNodeClientForm::close);
    }

    void VMTDNodeClientForm::updateView()
    {
        if (m_client == nullptr)
        {
            ui->lbSocketState->setText("State: None");
            ui->lbOpen->setText("Opened: No");
            ui->lbLocalAdress->setText("Local IP: None");
            ui->lbPeerAdress->setText("Remote IP: None");

            setWindowTitle("VMTD Node-client: н/д:н/д");

            return;
        }

        const QString title = QString("VMTD Node-client: %1:%2")
                              .arg(m_settings->serverIp())
                              .arg(m_settings->serverPort());
        setWindowTitle(title);

        const auto socketState = m_client->socket()->state();

        ui->lbSocketState->setText("State: " + QVariant::fromValue(socketState).toString());

        if (socketState == QAbstractSocket::ConnectedState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(true);
            ui->lbOpen->setText("Opened: Yes");
        }
        else if (socketState == QAbstractSocket::ConnectingState)
        {
            ui->pbConnectSocket->setEnabled(false);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Opened: No");
        }
        else
        {
            ui->pbConnectSocket->setEnabled(true);
            ui->pbDisconnectSocket->setEnabled(false);
            ui->lbOpen->setText("Opened: No");
        }

        QString ipAdress;

        if (!m_client->socket()->localAddress().isNull())
        {
            ipAdress = QString("Local IP: %1:%2")
                       .arg(m_client->socket()->localAddress().toString())
                       .arg(m_client->socket()->localPort());
            ui->
            lbLocalAdress->setText(ipAdress);
        }
        else
        {
            ui->lbLocalAdress->setText("Local IP: None");
        }

        if (!m_client->socket()->peerAddress().isNull())
        {
            ipAdress = QString("Remote IP: %1:%2")
                       .arg(m_client->socket()->peerAddress().toString())
                       .arg(m_client->socket()->peerPort());
            ui->lbPeerAdress->setText(ipAdress);
        }
        else
        {
            ui->lbPeerAdress->setText("Remote IP: None");
        }
    }

    void VMTDNodeClientForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDNodeClientForm::pbShowDetailedStateClicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? ">" : "<");
    }
}

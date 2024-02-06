#include    "VMTDNodeServerForm.h"
#include "ui_VMTDNodeServerForm.h"

#include "../../VMTDRepo.h"

namespace VMTDLib
{
    VMTDNodeServerForm::VMTDNodeServerForm(QWidget *parent, VMTDNodeServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDNodeServerForm),
        m_server(server)
    {
        m_server->settings()->debugOut(VN_S(VMTDNodeServerForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(m_server, &VMTDNodeServer::showDebugSignal,
                this, &VMTDNodeServerForm::showDebugSlot);

        for (auto socket : m_server->WsClientSockets)
            appendClientSocketSlot(socket);

        connect(m_server, &VMTDNodeServer::clientConnectedSignal,
                this, &VMTDNodeServerForm::appendClientSocketSlot);
        connect(m_server, &VMTDNodeServer::clientDisconnectedSignal,
                this, &VMTDNodeServerForm::removeClientSocketSlot);

        initializeView();

        updateView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout,
                this, &VMTDNodeServerForm::uiTimerTickSlot);
        m_uiTimer.start(500);

        m_server->settings()->debugOut(VN_S(VMTDNodeServerForm) + " | Constructor finished");
    }

    VMTDNodeServerForm::~VMTDNodeServerForm()
    {
        m_server->settings()->debugOut(VN_S(VMTDNodeServerForm) + " | Destructor called");

        delete ui;

        m_server->settings()->debugOut(VN_S(VMTDNodeServerForm) + " | Destructor finished");
    }

    void VMTDNodeServerForm::showDebugSlot(QWebSocket *socket, const QTime &time, const QString &text)
    {
        if (socket == nullptr)
        {
            ui->pteFlow->appendPlainText(QString("\n[%1] %2\n")
                                         .arg(time.toString("hh:mm:ss:zzz"))
                                         .arg(text));
        }
    }

    void VMTDNodeServerForm::initializeView()
    {
        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDNodeServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDNodeServer::stopListenSlot);

        connect(ui->pbShowDetailedState, &QPushButton::clicked,
                this, &VMTDNodeServerForm::pbShowDetailedStateClicked);

        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNodeServerForm::close);
    }

    void VMTDNodeServerForm::updateView()
    {
        if (m_server == nullptr)
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Running: None");
            ui->lbLocalAdress->setText("Listening: None");
            ui->lbClientSockets->setText("Connected clients: None");

            ui->lbServerName->setText("Server name: None");

            setWindowTitle("VMTD Node-сервер: None");

            return;
        }

        const auto title = QString("VMTD Node-сервер: %1")
                           .arg(m_server->settings()->localPort());
        setWindowTitle(title);

        if (m_server->wsServer()->isListening())
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(true);
            ui->lbOpen->setText("Running: Yes");
            ui->lbLocalAdress->setText("Listening: "
                                       + QString::number(m_server->settings()->serverPort()));
            ui->lbClientSockets->setText("Connected clients: "
                                         + QString::number(m_server->WsClientSockets.size()));
            ui->lbServerName->setText("Server name: "
                                      + m_server->wsServer()->serverName());
        }
        else
        {
            ui->pbStart->setEnabled(true);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Running: No");
            ui->lbLocalAdress->setText("Listening: No");
            ui->lbClientSockets->setText("Connected clients: None");
            ui->lbServerName->setText("Server name: None");
        }
    }

    void VMTDNodeServerForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDNodeServerForm::appendClientSocketSlot(QWebSocket *socket)
    {
        auto form = new VMTDNodeAdapterForm(this, socket);

        connect(m_server, &VMTDNodeServer::showDebugSignal,
                form, &VMTDNodeAdapterForm::showDebugSlot);

        m_socketToForm.insert(socket, form);

        const QString tabTitle = QString("%1:%2")
                                 .arg(QHostAddress(socket->peerAddress().toIPv4Address()).toString())
                                 .arg(socket->peerPort());

        ui->twSockets->addTab(form, tabTitle);
    }
    void VMTDNodeServerForm::removeClientSocketSlot(QWebSocket *socket)
    {
        auto form = m_socketToForm.value(socket);

        if (form != nullptr)
        {
            m_socketToForm.remove(socket);
            delete form;
        }
    }

    void VMTDNodeServerForm::pbShowDetailedStateClicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? ">" : "<");
    }
}

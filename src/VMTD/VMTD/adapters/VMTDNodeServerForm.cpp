#include    "VMTDNodeServerForm.h"
#include "ui_VMTDNodeServerForm.h"

namespace VMTDLib
{
    VMTDNodeServerForm::VMTDNodeServerForm(QWidget *parent, VMTDNodeServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDNodeServerForm),
        m_server(server)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);

        initializeView();

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
    }

    VMTDNodeServerForm::~VMTDNodeServerForm()
    {
        delete ui;
    }

    void VMTDNodeServerForm::showDebugSlot(QWebSocket *socket, const QString &text)
    {
        if (socket == nullptr)
        {
            ui->pteFlow->appendPlainText("\n--------------------------------\n");
            ui->pteFlow->appendPlainText(text);
            ui->pteFlow->appendPlainText("\n--------------------------------\n");
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

        connect(form, &VMTDNodeAdapterForm::sendRequestSignal,
                m_server, &VMTDNodeServer::sendRequestSlot);

        m_socketToForm.insert(socket, form);

        const QString tabTitle = QString("%1:%2")
                                 .arg(socket->peerAddress().toString(),
                                      socket->peerPort());

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
        ui->pbShowDetailedState->setText(ui->wRight->isVisible() ? "<" : ">");
    }

    void VMTDNodeServerForm::pbClearErrorsClicked()
    {
        m_server->clearServerErrors();
    }

    void VMTDNodeServerForm::initializeView()
    {
        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDNodeServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDNodeServer::stopListenSlot);

        connect(ui->pbClearErrors, &QPushButton::clicked,
                this, &VMTDNodeServerForm::pbClearErrorsClicked);

        connect(ui->pbShowDetailedState, &QPushButton::clicked,
                this, &VMTDNodeServerForm::pbShowDetailedStateClicked);
    }

    void VMTDNodeServerForm::updateView()
    {
        if (m_server == nullptr)
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Сервер запущен: н/д");
            ui->lbLocalAdress->setText("Слушает порт: н/д");
            ui->lbClientSockets->setText("Подключенные клиенты: н/д");

            ui->lbServerName->setText("Имя сервера: н/д");

            setWindowTitle("VMTD Node-сервер: н/д");

            return;
        }

        const auto title = QString("VMTD Node-сервер: %1")
                .arg(m_server->settings()->localPort());
        setWindowTitle(title);

        if (m_server->wsServer()->isListening())
        {
            ui->pbStart->setEnabled(false);
            ui->pbStop->setEnabled(true);
            ui->lbOpen->setText("Сервер запущен: Да");
            ui->lbLocalAdress->setText("Слушает порт: "
                                       + QString::number(m_server->settings()->localPort()));
            ui->lbClientSockets->setText("Подключенные клиенты: "
                                         + QString::number(m_server->WsClientSockets.size()));
            ui->lbServerName->setText("Имя сервера: "
                                      + m_server->wsServer()->serverName());
        }
        else
        {
            ui->pbStart->setEnabled(true);
            ui->pbStop->setEnabled(false);
            ui->lbOpen->setText("Сервер запущен: Нет");
            ui->lbLocalAdress->setText("Слушает порт: н/д");
            ui->lbClientSockets->setText("Подключенные клиенты: н/д");
            ui->lbServerName->setText("Имя сервера: н/д");
        }

        if (ui->pteErrors->toPlainText() != m_server->serverErrors())
            ui->pteErrors->appendPlainText(m_server->serverErrors());
    }
}

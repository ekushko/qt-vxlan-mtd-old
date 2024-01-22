#include    "VMTDNxApiServerForm.h"
#include "ui_VMTDNxApiServerForm.h"

#include "VMTDNxApiAdapterForm.h"

namespace VMTDLib
{
    VMTDNxApiServerForm::VMTDNxApiServerForm(QWidget *parent, VMTDNxApiServer *server) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiServerForm),
        m_server(server)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("NX-API Adapters");

        connect(m_server, &VMTDNxApiServer::adapterCreatedSignal,
                this, &VMTDNxApiServerForm::adapterCreatedSlot);

        for (auto adapter : m_server->adapters())
            adapterCreatedSlot(adapter);

        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::stopListenSlot);

        connect(ui->pbHideRight, &QPushButton::clicked,
                this, &VMTDNxApiServerForm::pbHideRightClicked);

        connect(ui->pbClose, &QPushButton::clicked,
                this, &VMTDNxApiServerForm::close);

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout,
                this, &VMTDNxApiServerForm::uiTimerTickSlot);
        m_uiTimer.start(500);

        uiTimerTickSlot();
    }

    VMTDNxApiServerForm::~VMTDNxApiServerForm()
    {
        delete ui;
    }

    void VMTDNxApiServerForm::uiTimerTickSlot()
    {
        ui->pbStart->setEnabled(!m_server->isListening());
        ui->pbStop->setEnabled(m_server->isListening());
        ui->lbListening->setText(QString("Listening: ") + (m_server->isListening() ? "Yes" : "No"));
        ui->lbAdapters->setText(QString("Adapters: %1").arg(m_server->adapters().size()));
    }

    void VMTDNxApiServerForm::adapterCreatedSlot(VMTDNxApiAdapter *adapter)
    {
        auto form = new VMTDNxApiAdapterForm(ui->twAdapters, adapter);
        ui->twAdapters->addTab(form, adapter->url().toString());
    }

    void VMTDNxApiServerForm::pbHideRightClicked()
    {
        ui->wRight->setVisible(!ui->wRight->isVisible());
        ui->pbHideRight->setText(ui->wRight->isVisible() ? "<" : ">");
    }
}

#include    "VMTDNxApiServerForm.h"
#include "ui_VMTDNxApiServerForm.h"

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

        connect(ui->pbRefresh, &QPushButton::clicked,
                this, &VMTDNxApiServerForm::updateAdaptersListSlot);

        updateAdaptersListSlot();

        connect(ui->pbStart, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::startListenSlot);
        connect(ui->pbStop, &QPushButton::clicked,
                m_server, &VMTDNxApiServer::stopListenSlot);

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
    }

    void VMTDNxApiServerForm::updateAdaptersListSlot()
    {
        ui->lwAdapters->clear();

        for (int i = 0; i < m_server->adapters().size(); ++i)
            ui->lwAdapters->addItem(m_server->adapters().at(i)->url().toString());
    }
}

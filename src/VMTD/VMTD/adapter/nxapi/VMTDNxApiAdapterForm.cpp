#include    "VMTDNxApiAdapterForm.h"
#include "ui_VMTDNxApiAdapterForm.h"

#include "../../VMTDRepo.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

namespace VMTDLib
{
    VMTDNxApiAdapterForm::VMTDNxApiAdapterForm(QWidget *parent, VMTDNxApiAdapter *adapter) :
        QWidget(parent),
        ui(new Ui::VMTDNxApiAdapterForm),
        m_adapter(adapter),
        m_settings(adapter->settings())
    {
        m_settings->debugOut(VN_S(VMTDNxApiAdapterForm) + " | Constructor called");

        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        setWindowTitle(QString("NX-API Адаптер (%1)").arg(m_adapter->url().toString()));
        setAttribute(Qt::WA_DeleteOnClose, true);

        connect(m_adapter, &VMTDNxApiAdapter::showDebugSignal,
                this, &VMTDNxApiAdapterForm::showDebugSlot,
                Qt::QueuedConnection);

        initializeView();
        updateView();

        m_settings->debugOut(VN_S(VMTDNxApiAdapterForm) + " | Constructor finished");
    }

    VMTDNxApiAdapterForm::~VMTDNxApiAdapterForm()
    {
        m_settings->debugOut(VN_S(VMTDNxApiAdapterForm) + " | Destructor called");

        delete ui;

        m_settings->debugOut(VN_S(VMTDNxApiAdapterForm) + " | Destructor finished");
    }

    void VMTDNxApiAdapterForm::updateView()
    {
        ui->lbUrl->setText(m_adapter->url().toString());
    }

    void VMTDNxApiAdapterForm::showDebugSlot(const QTime &time, const QString &text)
    {
        m_adapterForm->appendText(QString("\n[%1] %2\n")
                                  .arg(time.toString("hh:mm:ss:zzz"))
                                  .arg(text));
    }

    void VMTDNxApiAdapterForm::initializeView()
    {
        m_adapterForm = new VMTDAdapterForm(ui->wAdapter);
    }
}

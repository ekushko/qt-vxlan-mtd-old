#include    "VMTDSwitchForm.h"
#include "ui_VMTDSwitchForm.h"

namespace VMTDLib
{
    VMTDSwitchForm::VMTDSwitchForm(QWidget *parent, VMTDModel *model, int identificator) :
        QWidget(parent),
        ui(new Ui::VMTDSwitchForm),
        m_model(model),
        m_sw(m_model->sw(identificator)),
        m_identificator(identificator)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Switch (id: %1) [%2]")
                       .arg(m_sw->identificator())
                       .arg(m_sw->url().toString()));

        initializeView();

        setEditMode(false);
        updateView();
    }

    VMTDSwitchForm::~VMTDSwitchForm()
    {
        delete ui;
    }

    void VMTDSwitchForm::initializeView()
    {
        connect(ui->sbPortCount, qOverload<int>(&QSpinBox::valueChanged),
                this, &VMTDSwitchForm::sbPortCountValueChanged);

        connect(ui->pbRefresh, &QPushButton::clicked, this, &VMTDSwitchForm::pbRefreshClicked);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDSwitchForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDSwitchForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDSwitchForm::pbCancelClicked);
    }

    void VMTDSwitchForm::setEditMode(bool isEditMode)
    {
        if (isEditMode && m_model->isReadOnly())
            return;

        m_isEditMode = isEditMode;

        ui->pbChange->setEnabled(!m_isEditMode && !m_model->isReadOnly());
        ui->pbAccept->setEnabled(m_isEditMode);
        ui->pbCancel->setEnabled(m_isEditMode);

        ui->wMain->setEnabled(m_isEditMode);
    }

    void VMTDSwitchForm::updateView()
    {
        ui->lbOnline->setText(m_sw->isOnline() ? "Yes" : "No");
        ui->leIdentificator->setText(QString::number(m_identificator));
        ui->leUrl->setText(m_sw->url().toString());
        ui->sbPortCount->setValue(m_sw->portCount());
    }

    void VMTDSwitchForm::updateData()
    {
        m_identificator = ui->leIdentificator->text().toInt();
        m_sw->setIdentificator(m_identificator);
        m_sw->setUrl(QUrl(ui->leUrl->text()));
        m_sw->setPortCount(ui->sbPortCount->value());

        for (auto i = 0; i < m_sw->portCount(); ++i)
        {
            m_sw->PortToNode[i] = m_cbPortNodes.at(i)->currentData().toInt();
            m_sw->PortToInterface[i] = m_lePortInterfaces.at(i)->text();
        }
    }

    void VMTDSwitchForm::sbPortCountValueChanged(int value)
    {
        if (m_wPorts.count() != value)
        {
            qDeleteAll(m_wPorts);
            m_wPorts.clear();

            for (auto i = 0; i < value; ++i)
            {
                auto w = new QWidget(ui->wPorts);
                ui->wPorts->layout()->addWidget(w);
                m_wPorts.append(w);

                auto lb = new QLabel(QString("Port %1:").arg(i + 1), w);

                auto le = new QLineEdit(w);
                m_lePortInterfaces.append(le);

                auto cb = new QComboBox(w);
                cb->addItem("None", -1);
                for (auto node : m_model->nodes().values())
                    cb->addItem(node->ip(), node->identificator());
                m_cbPortNodes.append(cb);

                 auto l = new QHBoxLayout(w);
                 l->addWidget(lb);
                 l->addWidget(cb);
            }
        }

        for (auto i = 0; i < m_wPorts.size(); ++i)
        {
            auto identificator = m_sw->PortToNode.at(i);

            auto cb = m_cbPortNodes.at(i);
            cb->setCurrentIndex(cb->findData(identificator));

            auto le = m_lePortInterfaces.at(i);
            le->setText(m_sw->PortToInterface.at(i));
        }
    }

    void VMTDSwitchForm::pbRefreshClicked()
    {
        updateView();
    }

    void VMTDSwitchForm::pbChangeClicked()
    {
        setEditMode(true);
        updateView();
    }
    void VMTDSwitchForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();
    }
    void VMTDSwitchForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();
    }
}

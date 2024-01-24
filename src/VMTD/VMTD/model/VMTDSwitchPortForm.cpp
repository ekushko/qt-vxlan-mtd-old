#include    "VMTDSwitchPortForm.h"
#include "ui_VMTDSwitchPortForm.h"

#include <QLabel>
#include <QLayout>
#include <QGridLayout>

namespace VMTDLib
{
    VMTDSwitchPortForm::VMTDSwitchPortForm(QWidget *parent, VMTDModel *model, int id) :
        QWidget(parent),
        ui(new Ui::VMTDSwitchPortForm),
        m_model(model),
        m_sw(m_model->sw(id)),
        m_id(id)
    {
        ui->setupUi(this);

        if (parent != nullptr && parent->layout() != nullptr)
            parent->layout()->addWidget(this);

        initializeView();
        updateView();
    }

    VMTDSwitchPortForm::~VMTDSwitchPortForm()
    {
        delete ui;
    }

    void VMTDSwitchPortForm::reinitializeView()
    {
        qDeleteAll(m_wPorts);
        m_wPorts.clear();
        m_lePortInterfaces.clear();
        m_cbPortNodes.clear();

        for (auto i = 0; i < m_sw->portCount(); ++i)
        {
            auto w = new QWidget(ui->wPorts);
            dynamic_cast<QGridLayout *>(ui->wPorts->layout())->addWidget(w, i / 2, i % 2);
            m_wPorts.append(w);

            auto lb = new QLabel(QString("Port %1:").arg(i + 1), w);
            lb->setFixedWidth(50);

            auto le = new QLineEdit(w);
            le->setFixedWidth(60);
            m_lePortInterfaces.append(le);

            auto cb = new QComboBox(w);
            cb->addItem("None", -1);

            for (auto node : m_model->nodes().values())
                cb->addItem(node->ip(), node->id());

            m_cbPortNodes.append(cb);

            auto l = new QHBoxLayout(w);
            l->addWidget(lb);
            l->addWidget(le);
            l->addWidget(cb);
        }
    }

    void VMTDSwitchPortForm::updateView()
    {
        for (auto i = 0; i < m_sw->portCount(); ++i)
        {
            const auto id = m_sw->PortToNode.at(i);

            auto cb = m_cbPortNodes.at(i);
            cb->setCurrentIndex(cb->findData(id));

            auto le = m_lePortInterfaces.at(i);
            le->setText(m_sw->PortToInterface.at(i));
        }
    }

    void VMTDSwitchPortForm::updateData()
    {
        for (auto i = 0; i < m_sw->portCount(); ++i)
        {
            m_sw->PortToNode[i] = m_cbPortNodes.at(i)->currentData().toInt();
            m_sw->PortToInterface[i] = m_lePortInterfaces.at(i)->text();
        }
    }

    void VMTDSwitchPortForm::initializeView()
    {
        reinitializeView();
    }
}

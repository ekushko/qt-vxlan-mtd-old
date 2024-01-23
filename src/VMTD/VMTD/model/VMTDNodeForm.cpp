#include    "VMTDNodeForm.h"
#include "ui_VMTDNodeForm.h"

namespace VMTDLib
{
    VMTDNodeForm::VMTDNodeForm(QWidget *parent, VMTDModel *model, int identificator) :
        QWidget(parent),
        ui(new Ui::VMTDNodeForm),
        m_model(model),
        m_node(m_model->node(identificator)),
        m_identificator(identificator)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle(QString("Node (id: %1) [%2]")
                       .arg(m_node->identificator())
                       .arg(m_node->ip()));

        initializeView();

        m_uiTimer.setParent(this);
        connect(&m_uiTimer, &QTimer::timeout, this, &VMTDNodeForm::uiTimerTickSlot);
        m_uiTimer.start(1000);

        setEditMode(false);
        updateView();
    }

    VMTDNodeForm::~VMTDNodeForm()
    {
        delete ui;
    }

    void VMTDNodeForm::initializeView()
    {
        ui->cbCurrentSwitch->addItem("None", -1);

        for (auto sw : m_model->switches().values())
            ui->cbCurrentSwitch->addItem(sw->url().toString(), sw->identificator());

        connect(ui->cbCurrentSwitch, qOverload<int>(&QComboBox::currentIndexChanged),
                this, &VMTDNodeForm::cbCurrentSwitchIndexChanged);

        connect(ui->pbChange, &QPushButton::clicked, this, &VMTDNodeForm::pbChangeClicked);
        connect(ui->pbAccept, &QPushButton::clicked, this, &VMTDNodeForm::pbAcceptClicked);
        connect(ui->pbCancel, &QPushButton::clicked, this, &VMTDNodeForm::pbCancelClicked);
    }

    void VMTDNodeForm::setEditMode(bool isEditMode)
    {
        if (isEditMode && m_model->isReadOnly())
            return;

        m_isEditMode = isEditMode;

        ui->pbChange->setEnabled(!m_isEditMode && !m_model->isReadOnly());
        ui->pbAccept->setEnabled(m_isEditMode);
        ui->pbCancel->setEnabled(m_isEditMode);

        ui->wMain->setEnabled(m_isEditMode);
    }

    void VMTDNodeForm::updateView()
    {
        ui->lbOnline->setText(m_node->isOnline() ? "Yes" : "No");
        ui->leIdentificator->setText(QString::number(m_identificator));
        ui->leIp->setText(m_node->ip());
        ui->sbPortNumber->setValue(m_node->portNumber());
        ui->cbCurrentSwitch->setCurrentIndex(ui->cbCurrentSwitch->findData(m_node->currentSwitch()));
    }

    void VMTDNodeForm::updateData()
    {
        m_identificator = ui->leIdentificator->text().toInt();
        m_node->setIdentificator(m_identificator);
        m_node->setIp(ui->leIp->text());
        m_node->setPortNumber(ui->sbPortNumber->value());
        m_node->setCurrentSwitch(ui->cbCurrentSwitch->currentData().toInt());
    }

    void VMTDNodeForm::uiTimerTickSlot()
    {
        updateView();
    }

    void VMTDNodeForm::cbCurrentSwitchIndexChanged(int index)
    {
        Q_UNUSED(index)

        auto sw = m_model->sw(ui->cbCurrentSwitch->currentData().toInt());
        ui->sbPortNumber->setRange(0, sw->portCount() - 1);
    }

    void VMTDNodeForm::pbChangeClicked()
    {
        m_uiTimer.stop();

        setEditMode(true);
        updateView();
    }
    void VMTDNodeForm::pbAcceptClicked()
    {
        updateData();

        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
    void VMTDNodeForm::pbCancelClicked()
    {
        setEditMode(false);
        updateView();

        m_uiTimer.start();
    }
}

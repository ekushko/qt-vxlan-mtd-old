#include    "VMTDMainForm.h"
#include "ui_VMTDMainForm.h"

#include "VMTDBuildInfo.h"

namespace VMTDLib
{
    VMTDMainForm::VMTDMainForm(QWidget *parent, VMTDController *controller) :
        QWidget(parent),
        ui(new Ui::VMTDMainForm),
        m_controller(controller)
    {
        ui->setupUi(this);

        setAttribute(Qt::WA_DeleteOnClose, true);
        setWindowTitle("Main");

        m_controller->settings()->setMainWidget(ui->stackedWidget);
        m_controller->showFormSlot();

        connect(m_controller->form(), &QWidget::destroyed,
                this, &VMTDMainForm::close);

        ui->lbVersion->setText(QString("Library %1: %2 from %3")
                               .arg(VMTDBuildInfo::filename())
                               .arg(VMTDBuildInfo::version())
                               .arg(VMTDBuildInfo::dateTime()
                                    .toString("dd-MM-yyyy hh:mm:ss")));
    }

    VMTDMainForm::~VMTDMainForm()
    {
        m_controller->settings()->setMainWidget(nullptr);

        delete ui;
    }
}

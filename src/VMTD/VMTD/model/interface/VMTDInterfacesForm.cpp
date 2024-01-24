#include "VMTDInterfacesForm.h"
#include "ui_VMTDInterfacesForm.h"

VMTDInterfacesForm::VMTDInterfacesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VMTDInterfacesForm)
{
    ui->setupUi(this);
}

VMTDInterfacesForm::~VMTDInterfacesForm()
{
    delete ui;
}

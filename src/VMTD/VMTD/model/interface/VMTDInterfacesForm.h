#pragma once

#include <QWidget>

namespace Ui {
    class VMTDInterfacesForm;
}

class VMTDInterfacesForm : public QWidget
{
    Q_OBJECT

public:
    explicit VMTDInterfacesForm(QWidget *parent = nullptr);
    ~VMTDInterfacesForm();

private:
    Ui::VMTDInterfacesForm *ui;
};


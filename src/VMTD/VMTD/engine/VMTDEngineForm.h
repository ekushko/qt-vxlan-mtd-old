#pragma once

#include "VMTDEngine.h"

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui
{
    class VMTDEngineForm;
}

namespace VMTDLib
{
    class VMTDEngineForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDEngineForm(QWidget *parent, VMTDEngine *engine);
        ~VMTDEngineForm();

    private:

        void initializeView();
        void     updateView();

        Ui::VMTDEngineForm *ui;

        VMTDEngine *m_engine;

        VMTDSettings *m_settings;

    private slots:

        void mixedSlot();

        void trwGroupsItemDoubleClicked(QTreeWidgetItem *item, int column);
    };
}

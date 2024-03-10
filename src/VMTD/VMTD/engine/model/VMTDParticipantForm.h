#pragma once

#include "VMTDParticipant.h"

#include <QWidget>
#include <QLabel>
#include <QTimer>

namespace Ui
{
    class VMTDParticipantForm;
}

namespace VMTDLib
{
    class VMTDParticipantForm : public QWidget
    {
        Q_OBJECT

    public:

        VMTDParticipantForm(QWidget *parent, VMTDParticipant *participant);
        ~VMTDParticipantForm();

    private:

        void initializeView();
        void     updateView();

        Ui::VMTDParticipantForm *ui;

        VMTDParticipant *m_participant;
        VMTDSettings *m_settings;

        QTimer m_uiTimer;

        QList<QLabel *> m_hostForms;

    private slots:

        void uiTimerTickSlot();
    };
}

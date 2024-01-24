#pragma once

#include "VMTDInterface.h"

#include <QMap>

namespace VMTDLib
{
    class VMTDInterfacesForm;

    class VMTD_SHARED VMTDInterfaces : public QObject
    {
        Q_OBJECT

    public:

        VMTDInterfaces(QObject *parent, VMTDSettings *settings);

        const QMap<int, VMTDInterface *> &interfaces() const;
        VMTDInterface *interface(int id) const;
        void        addInterface(VMTDInterface *interface);
        void     removeInterface(int id);

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDInterfacesForm> m_form;

        VMTDSettings *m_settings;

        QMap<int, VMTDInterface *> m_interfaces;
    };
}

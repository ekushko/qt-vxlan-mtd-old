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

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        bool    onlyOneMode() const;
        void setOnlyOneMode(bool onlyOneMode);

        const QMap<int, VMTDInterface *> &interfaces() const;
        VMTDInterface *interface(int id) const;
        bool        addInterface();
        bool     removeInterface(int id);

    signals:

        void interfaceCreatedSignal(int id);
        void interfaceRemovedSignal(int id);

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDInterfacesForm> m_form;

        VMTDSettings *m_settings;

        bool m_onlyOneMode = true;

        int m_idCounter = 0;
        QMap<int, VMTDInterface *> m_interfaces;
    };
}

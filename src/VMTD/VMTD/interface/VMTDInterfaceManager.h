#pragma once

#include "VMTDInterface.h"

#include <QMap>

namespace VMTDLib
{
    class VMTDInterfaceManagerForm;

    class VMTD_SHARED VMTDInterfaceManager : public QObject
    {
        Q_OBJECT

    public:

        VMTDInterfaceManager(QObject *parent, VMTDSettings *settings);
        ~VMTDInterfaceManager();

        VMTDSettings *settings() const;

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        bool    onlyOneMode() const;
        void setOnlyOneMode(bool onlyOneMode);

        bool contains(VMTDInterface *interface) const;

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

        QPointer<VMTDInterfaceManagerForm> m_form;

        VMTDSettings *m_settings;

        bool m_onlyOneMode = false;

        QMap<int, VMTDInterface *> m_interfaces;
    };
}

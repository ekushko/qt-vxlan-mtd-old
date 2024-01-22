#pragma once

#include "../VMTDSettings.h"

#include "VMTDSwitch.h"
#include "VMTDNode.h"

namespace VMTDLib
{
    class VMTDModelForm;

    class VMTD_SHARED VMTDModel : public QObject
    {
        Q_OBJECT

    public:

        VMTDModel(QObject *parent, VMTDSettings *settings);

        VMTDSettings *settings() const;

        // ЛОГИКА

        QJsonObject toJson() const;
        void      fromJson(const QJsonObject &jsonObj);

        void connectNodeToSwitch(VMTDNode *n, VMTDSwitch *sw, int portNumber);

        // МЕТАДАННЫЕ

        bool  isReadOnly() const;
        void setReadOnly(bool isReadOnly);

        // ДАННЫЕ

        bool isSwitchExist(int identificator) const;
        VMTDSwitch *sw(int identificator) const;
        VMTDSwitch *sw(const QUrl &url) const;
        const QMap<int, VMTDSwitch *> &switches() const;
        bool addSwitch(VMTDSwitch *sw);
        void removeSwitch(int identificator);

        bool isNodeExist(int identificator) const;
        VMTDNode *node(int identificator) const;
        VMTDNode *node(const QString &ip) const;
        const QMap<int, VMTDNode *> &nodes() const;
        bool addNode(VMTDNode *node);
        void removeNode(int identificator);

    public slots:

        void showFormSlot();

    private:

        QPointer<VMTDModelForm> m_form;

        VMTDSettings *m_settings;

        bool m_isReadOnly;

        QMap<int, VMTDSwitch *> m_switches;

        QMap<int, VMTDNode *> m_nodes;
    };
}

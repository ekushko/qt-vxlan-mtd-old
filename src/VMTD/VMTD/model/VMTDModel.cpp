#include "VMTDModel.h"
#include "VMTDModelForm.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDModel::VMTDModel(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
        , m_isReadOnly(false)
    {
        loadSlot();
    }

    VMTDSettings *VMTDModel::settings() const
    {
        return m_settings;
    }

    QJsonObject VMTDModel::toJson() const
    {
        QJsonObject jsonObj;

        QJsonArray switchesArr, nodesArr;

        for (auto sw : m_switches.values())
            switchesArr.append(sw->toJson());

        jsonObj["switches"] = switchesArr;

        for (auto node : m_nodes.values())
            nodesArr.append(node->toJson());

        jsonObj["nodes"] = nodesArr;

        return jsonObj;
    }
    void VMTDModel::fromJson(const QJsonObject &jsonObj)
    {
        qDeleteAll(m_switches.values());
        m_switches.clear();

        qDeleteAll(m_nodes.values());
        m_nodes.clear();

        if (jsonObj.isEmpty())
            return;

        auto switchesArr = jsonObj["switches"].toArray();

        for (int i = 0; i < switchesArr.size(); ++i)
        {
            auto sw = new VMTDSwitch(this, m_settings);
            sw->fromJson(switchesArr[i].toObject());
            m_switches[sw->id()] = sw;
        }

        auto nodesArr = jsonObj["nodes"].toArray();

        for (int i = 0; i < nodesArr.size(); ++i)
        {
            auto node = new VMTDNode(this, m_settings);
            node->fromJson(nodesArr[i].toObject());
            m_nodes[node->id()] = node;
        }
    }

    void VMTDModel::connectNodeToSwitch(VMTDNode *n, VMTDSwitch *sw, int portNumber)
    {
        const auto oldNodeId = sw->PortToNode.at(portNumber);

        if (oldNodeId >= 0)
        {
            auto oldNode = node(oldNodeId);
            oldNode->setCurrentSwitch(-1);
        }

        n->setCurrentSwitch(sw->id());
        sw->PortToNode[portNumber] = n->id();
    }

    bool VMTDModel::isReadOnly() const
    {
        return m_isReadOnly;
    }
    void VMTDModel::setReadOnly(bool isReadOnly)
    {
        m_isReadOnly = isReadOnly;
    }

    bool VMTDModel::isSwitchExist(int id) const
    {
        return m_switches.contains(id);
    }
    VMTDSwitch *VMTDModel::sw(int id) const
    {
        return m_switches.value(id, nullptr);
    }
    VMTDSwitch *VMTDModel::sw(const QUrl &url) const
    {
        for (auto sw_ : m_switches.values())
        {
            if (sw_->url().toString(QUrl::RemoveUserInfo)
                == url.toString(QUrl::RemoveUserInfo))
                return sw_;
        }

        return nullptr;
    }
    const QMap<int, VMTDSwitch *> &VMTDModel::switches() const
    {
        return m_switches;
    }
    bool VMTDModel::addSwitch(VMTDSwitch *sw)
    {
        if (isSwitchExist(sw->id()))
            return false;

        sw->setParent(this);
        connect(sw, &VMTDSwitch::updatedSignal, this, &VMTDModel::updatedSwSlot);
        m_switches[sw->id()] = sw;
        return true;
    }
    void VMTDModel::removeSwitch(int id)
    {
        if (!isSwitchExist(id))
            return;

        delete m_switches[id];
        m_switches.remove(id);
    }

    bool VMTDModel::isNodeExist(int id) const
    {
        return m_nodes.contains(id);
    }
    VMTDNode *VMTDModel::node(int id) const
    {
        return m_nodes.value(id, nullptr);
    }
    VMTDNode *VMTDModel::node(const QString &ip) const
    {
        for (auto node_ : m_nodes.values())
        {
            if (node_->ip() == ip)
                return node_;
        }

        return nullptr;
    }
    const QMap<int, VMTDNode *> &VMTDModel::nodes() const
    {
        return m_nodes;
    }
    bool VMTDModel::addNode(VMTDNode *node)
    {
        if (isNodeExist(node->id()))
            return false;

        node->setParent(this);
        connect(node, &VMTDNode::updatedSignal, this, &VMTDModel::updatedNodeSlot);
        m_nodes[node->id()] = node;
        return true;
    }
    void VMTDModel::removeNode(int id)
    {
        if (!isNodeExist(id))
            return;

        delete m_nodes[id];
        m_nodes.remove(id);
    }

    void VMTDModel::showFormSlot(QWidget *parent)
    {
        if (m_form == nullptr)
            m_form = new VMTDModelForm(parent, this);

        m_form->show();
        m_form->raise();
        m_form->activateWindow();
    }

    void VMTDModel::saveSlot()
    {
        m_settings->setModelObj(toJson());
        m_settings->save();
    }
    void VMTDModel::loadSlot()
    {
        fromJson(m_settings->modelObj());
    }

    void VMTDModel::updatedNodeSlot()
    {
        auto n = dynamic_cast<VMTDNode *>(sender());

        if (n == nullptr)
            return;

        auto s = sw(n->currentSwitch());

        if (s == nullptr)
            return;

        connectNodeToSwitch(n, s, n->portNumber());
    }
    void VMTDModel::updatedSwSlot()
    {
        auto s = dynamic_cast<VMTDSwitch *>(sender());

        if (s == nullptr)
            return;

        for (int i = 0; i < s->PortToNode.size(); ++i)
        {
            auto n = node(s->PortToNode[i]);

            if (n == nullptr)
                continue;

            connectNodeToSwitch(n, s, i);
        }
    }
}

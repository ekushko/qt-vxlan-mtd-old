#include "VMTDModel.h"

#include <QJsonArray>

namespace VMTDLib
{
    VMTDModel::VMTDModel(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
        , m_isReadOnly(false)
    {

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
            m_switches[sw->identificator()] = sw;
        }

        auto nodesArr = jsonObj["nodes"].toArray();
        for (int i = 0; i < nodesArr.size(); ++i)
        {
            auto node = new VMTDNode(this, m_settings);
            node->fromJson(nodesArr[i].toObject());
            m_nodes[node->identificator()] = node;
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

        n->setCurrentSwitch(sw->identificator());
        sw->PortToNode[portNumber] = n->identificator();
    }

    bool VMTDModel::isReadOnly() const
    {
        return m_isReadOnly;
    }
    void VMTDModel::setReadOnly(bool isReadOnly)
    {
        m_isReadOnly = isReadOnly;
    }

    bool VMTDModel::isSwitchExist(int identificator) const
    {
        return m_switches.contains(identificator);
    }
    VMTDSwitch *VMTDModel::sw(int identificator) const
    {
        return m_switches.value(identificator, nullptr);
    }
    VMTDSwitch *VMTDModel::sw(const QUrl &url) const
    {
        for (auto sw_ : m_switches.values())
        {
            if (sw_->url() == url)
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
        if (isSwitchExist(sw->identificator()))
            return false;

        sw->setParent(this);
        m_switches[sw->identificator()] = sw;
        return true;
    }
    void VMTDModel::removeSwitch(int identificator)
    {
        if (!isSwitchExist(identificator))
            return;

        delete m_switches[identificator];
        m_switches.remove(identificator);
    }

    bool VMTDModel::isNodeExist(int identificator) const
    {
        return m_nodes.contains(identificator);
    }
    VMTDNode *VMTDModel::node(int identificator) const
    {
        return m_nodes.value(identificator, nullptr);
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
        if (isNodeExist(node->identificator()))
            return false;

        node->setParent(this);
        m_nodes[node->identificator()] = node;
        return true;
    }
    void VMTDModel::removeNode(int identificator)
    {
        if (!isNodeExist(identificator))
            return;

        delete m_nodes[identificator];
        m_nodes.remove(identificator);
    }
}

#pragma once

#include <QObject>
#include <QMetaEnum>
#include <QTime>

#include <limits>

namespace VMTDLib
{
    class VMTDRepo
    {
    public:
        VMTDRepo() = delete;

        // *INDENT-OFF*
        template<typename EnumType>
        static QMap<int, QString> mapOfStrings(const QMetaObject &metaObject,
                                               const char *enumName,
                                               const QString &(*codeToS)(const EnumType &enumType))
        {
            const QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator(enumName));

            QMap<int, QString> map;

            for (int i = 0; i < metaEnum.keyCount(); i++)
            {
                map.insert(metaEnum.value(i),
                           codeToS((EnumType)metaEnum.value(i)));
            }

            return map;
        }
        // *INDENT-ON*

        static int generateId()
        {
            return 1 + (qrand() % (std::numeric_limits<int>::max() - 1));
        }
    };

    static const QString VLAN_HEADER_TEMPLATE =
    {
        "vlans:\n"
    };

    static const QString VLAN_TEMPLATE =
    {
        "  vlan.%1:\n"
        "    id: %1\n"
        "    link: %2\n"
        "    addresses: [%3/%4]\n"
    };

    static const QString ROUTE_HEADER_TEMPLATE =
    {
        "    routes:\n"
    };

    static const QString ROUTE_TEMPLATE =
    {
        "      - to: %1/%2\n"
        "        via: %3\n"
        "        metric: %4\n"
    };

    static const QString NETWORK_TEMPLATE =
    {
        "100.100.%1.%2"
    };

#define MTH_SETUP_INTERFACE_1 VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_INTERFACE_1)
#define MTH_SETUP_INTERFACE_2 VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_INTERFACE_2)
#define MTH_SETUP_HOSTS VMTDMethod::methods().value(VMTDMethod::EnName::SETUP_HOSTS)
#define MTH_CLEAR_INTERFACE_1 VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_INTERFACE_1)
#define MTH_CLEAR_INTERFACE_2 VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_INTERFACE_2)
#define MTH_CLEAR_HOSTS VMTDMethod::methods().value(VMTDMethod::EnName::CLEAR_HOSTS)
#define MTH_CHECK_CONNECTION VMTDMethod::methods().value(VMTDMethod::EnName::CHECK_CONNECTION)

#define PRM_INTERFACE VMTDMethod::params().value(VMTDMethod::EnParamName::INTERFACE)
#define PRM_IP VMTDMethod::params().value(VMTDMethod::EnParamName::IP)
#define PRM_MASK VMTDMethod::params().value(VMTDMethod::EnParamName::MASK)
#define PRM_GATEWAY VMTDMethod::params().value(VMTDMethod::EnParamName::GATEWAY)
#define PRM_VLAN_ID VMTDMethod::params().value(VMTDMethod::EnParamName::VLAN_ID)
#define PRM_NETWORK VMTDMethod::params().value(VMTDMethod::EnParamName::NETWORK)
#define PRM_DOMAIN_NAME VMTDMethod::params().value(VMTDMethod::EnParamName::DOMAIN_NAME)
#define PRM_METRIC VMTDMethod::params().value(VMTDMethod::EnParamName::METRIC)
#define PRM_ROUTES VMTDMethod::params().value(VMTDMethod::EnParamName::ROUTES)
#define PRM_HOSTS VMTDMethod::params().value(VMTDMethod::EnParamName::HOSTS)

    class VMTDMethod
    {
    public:

        VMTDMethod() = delete;

        enum class EnName
        {
            SETUP_INTERFACE_1,
            SETUP_INTERFACE_2,
            SETUP_HOSTS,

            CLEAR_INTERFACE_1,
            CLEAR_INTERFACE_2,
            CLEAR_HOSTS,

            CHECK_CONNECTION
        };

        enum class EnParamName
        {
            INTERFACE,
            IP,
            MASK,
            GATEWAY,
            VLAN_ID,
            NETWORK,
            DOMAIN_NAME,
            METRIC,
            ROUTES,
            HOSTS
        };

        static const QMap<EnName, QString> &methods();

        static const QMap<EnParamName, QString> &params();

        static const QList<EnParamName> &methodParams(EnName methodName);

        static const QList<EnParamName> &subParams(EnParamName paramName);

        static bool checkParam(EnParamName paramName, const QJsonValue &jsonVal);
    };

#define VN(Variable) (#Variable)
#define VN_S(Variable) QString(#Variable)
#define VN_UP(Variable) QString(#Variable).replace(0, 1, QString(#Variable).at(0).toLower())
#define VN_ME(Variable) (QString(#Variable).remove("m_"))
#define VN_NS(Variable) (QString(#Variable).split("::")[1])
#define VN_NS_2(Variable) (QString(#Variable).split("::")[2])
#define VN_MT(Variable) (QString(#Variable).left(QString(#Variable).indexOf('(')))
#define LIO(V,C) QString(#V).lastIndexOf(C)
#define VN_MT_PTR(V) (VN_S(V).mid(LIO(V,">") + 1, LIO(V,"(") - (LIO(V,">") + 1)))
#define VN_MT_REF(V) (VN_S(V).mid(LIO(V,".") + 1, LIO(V,"(") - (LIO(V,".") + 1)))

#define S static const QString
    S S_QUESTIONS = "???";

#define RETURN_S(Variable) { S s(Variable); return s; }

#define RETURN_MAP(EnumType, codeToS) \
{ \
    static const QMap<int, QString> map \
        = VMTDRepo::mapOfStrings(staticMetaObject, #EnumType, codeToS); \
    return map; \
}

#define CB_FILL_MAP(Combo, Map) for (auto it = Map.begin(); it != Map.end(); ++it) Combo->addItem(it.value(), it.key())
#define CB_DATA_TOE(Combo, Enum) static_cast<Enum>(Combo->currentData().toInt())
#define CB_SELECT(Combo, Value) Combo->setCurrentIndex(Combo->findData(static_cast<int>(Value)))
}

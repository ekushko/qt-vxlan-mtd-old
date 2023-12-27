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

        static int generateIdentificator()
        {
            return 1 + (qrand() % (std::numeric_limits<int>::max() - 1));
        }
    };

#define VN(Variable) (#Variable)
#define VN_S(Variable) QString(#Variable)
#define VN_ME(Variable) (QString(#Variable).remove("m_"))

#define S static const QString
    S S_QUESTIONS = "???";

#define RETURN_S(Variable) { S s(Variable); return s; }

#define RETURN_MAP(EnumType, codeToS) \
{ \
    static const QMap<int, QString> map \
        = VMTDRepo::mapOfStrings(staticMetaObject, #EnumType, codeToS); \
    return map; \
}

}

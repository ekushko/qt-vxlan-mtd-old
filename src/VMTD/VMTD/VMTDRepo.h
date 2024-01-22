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

}

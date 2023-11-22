#include "VMTDRepo.h"

namespace VMTDLib
{
    QString VMTDRepo::convertByteMessage(bool isInput, const QTime &time, const QString &data)
    {
        QString record;

        if (isInput == true)
            record = "|i|";
        else
            record = "|o|";

        record += " (" + time.toString("mm:ss:zzz") + ")";

        QString byteData = data;

        for (int i = 2; i < byteData.size(); i = i + 2)
        {
            byteData.insert(i, ' ');
            i++;
        }

        record += byteData;
        record += "\n";

        return record;
    }
}

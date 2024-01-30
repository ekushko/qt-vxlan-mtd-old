#include "VMTDProtocolHandler.h"

namespace VMTDLib
{
    VMTDProtocolHandler::VMTDProtocolHandler(QObject *parent, VMTDSettings *settings)
        : QObject(parent)
        , m_settings(settings)
    {

    }
}

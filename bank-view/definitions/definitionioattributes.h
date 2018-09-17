#ifndef DEFINITIONIOATTRIBUTES_H
#define DEFINITIONIOATTRIBUTES_H

#include <QObject>

namespace IoAttributes {

Q_NAMESPACE

enum HeaderIOAttributes
{
    ID,
    HAL_ADDRESS,
    NAME,
    REGION,
    PLC_ADDRESS,
    PHYSICAL_ADDRESS,
    TERMINAL
};
Q_ENUM_NS(HeaderIOAttributes)

}

#endif // DEFINITIONIOATTRIBUTES_H

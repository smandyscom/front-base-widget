#ifndef DEFINITIONAUXILIARYKEYS_H
#define DEFINITIONAUXILIARYKEYS_H

#include <QObject>
#include <QMap>
namespace DEF_BASIC_DIMENSION {
Q_NAMESPACE
enum Headers
{
    __KEY,
    __VALUE,
    __GROUP
};
enum Keys
{
    TIME,
    LENGTH,
    TORQUE_RATIO,
};
Q_ENUM_NS(Keys)
Q_ENUM_NS(Headers)

QMap<Keys,qreal> Dimension;

}

namespace DEF_REGION {
Q_NAMESPACE
enum Headers
{
    ID,
    NAME,
};
Q_ENUM_NS(Headers)
}


#endif // DEFINITIONAUXILIARYKEYS_H

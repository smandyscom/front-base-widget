#ifndef DEFINITIONAUXILIARYKEYS_H
#define DEFINITIONAUXILIARYKEYS_H

#include <QObject>
#include <QMap>

#include <QTableView>
#include <QHeaderView>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <utilities.h>

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

namespace HEADER_STRUCTURE {
Q_NAMESPACE
enum Headers
{
    ID,
    NAME,
    //! Locale
    en_US,
    zh_TW,
};
Q_ENUM_NS(Headers)

class HeaderRender
{
public:
    HeaderRender() {}

    static void renderViewHeader(QSqlTableModel* reference, QTableView* view, Headers key=zh_TW, bool isStrech = true);
};

}

namespace AUTH {
Q_NAMESPACE
enum AuthRoles
{
    ROLE_OPERATOR,
    ROLE_ENGINEER,
    ROLE_DEVELOPER,
};
Q_ENUM_NS(AuthRoles)
}


#endif // DEFINITIONAUXILIARYKEYS_H

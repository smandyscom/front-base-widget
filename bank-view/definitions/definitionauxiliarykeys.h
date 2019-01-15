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
//!
//! \brief The ExtendedRoles enum
//! Specify Model addtional role
enum ExtendedRoles
{
    UserRole_MonitorOnOff = Qt::UserRole,
    UserRole_OverrideOnOff = UserRole_MonitorOnOff+1,
};

enum OperationModes
{
    STATE_AUTO,
    STATE_SEMI_AUTO,
    STATE_MANUAL,
};
Q_ENUM_NS(OperationModes)
}

namespace AUTH {
Q_NAMESPACE
enum AuthRoles
{
	PROP_AUTH=-1,
	PASSWORD=-2,

    ROLE_OPERATOR=0,
    ROLE_ENGINEER=1,
    ROLE_DEVELOPER=2,
};
Q_ENUM_NS(AuthRoles)
}


#endif // DEFINITIONAUXILIARYKEYS_H

#ifndef DEFINITIONMATERIALKEYS_H
#define DEFINITIONMATERIALKEYS_H

#include <QObject>

namespace MaterialKeys {
Q_NAMESPACE
enum TableNames
{
    REPORT_HEAD,
    REPORT_POLL,
    QUERY_HEAD,
    //Views`
    VIEW_REPORT_OVERVIEW,
    //HEADERS
    HEADER_REPORT_HEAD,
    HEADER_REPORT_OVERVIEW,
    //Reference
    VIEW_MAT_TABLE_HOUSING,
    VIEW_DISTINCT_WORKING_NUMBER,
    VIEW_DISTINCT_SHIFT,
    VIEW_DISTINCT_EID,
    VIEW_DISTINCT_DATE
};
enum QueryFields
{
    ID,
    en_US,
    zh_TW,
    VALUE,
};
enum QueryKeys
{
    HOUSING1 = 0,
    HOUSING2,
    WORKINGNUMBER,
    SHIFT,
    EID,
    REPORTDATE
};
Q_ENUM_NS(TableNames)
Q_ENUM_NS(QueryFields)
Q_ENUM_NS(QueryKeys)
}

#endif // DEFINITIONMATERIALKEYS_H

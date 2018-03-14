#include "tablemodelaxis.h"

TableModelAxis::TableModelAxis(QSqlTableModel *instance) :
    QSqlTableModel(instance->parent()),
    __instance(instance)
{

}

QVariant TableModelAxis::Value(int id, Headers header) const
{
    //__instance.sort(ID,Qt::AscendingOrder);
    return __instance->index(id,header).data();
}

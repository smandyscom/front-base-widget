#include "tablemodeliooverride.h"

TableModelIOOverride::TableModelIOOverride(QSqlRelationalTableModel *source):
    QSqlRelationalTableModel(source->parent())
{
    //!
    //!
    setTable(source->tableName());
    select();

    __channel = ModbusChannel::Instance();
    //! Forced to inform View to update periodically
    __timer = new QTimer(this);
    connect(__timer,&QTimer::timeout,[=](){
       emit dataChanged(index(0,0),index(rowCount(),columnCount()));
    });
    __timer->start(100);
}

//!
//! \brief TableModelIOOverride::data
//! \param idx
//! \param role
//! \return
//!
QVariant TableModelIOOverride::data(const QModelIndex &idx, int role) const
{
    //!Base method
    if(!(idx.column()== fieldIndex(QVariant::fromValue(NAME).toString()) && role==Qt::BackgroundRole))
        return QSqlTableModel::data(idx,role);

    //!Change color once the background role request comes
    if(__channel->Access<bool>(CurrentIndexAddress(idx.row())))
        return QColor(Qt::green);
    else
        return QColor(Qt::gray);
}

//!
//! \brief TableModelIOOverride::CurrentIndexAddress
//! \param rowIndex
//! \return
//!
ModbusDriverAddress TableModelIOOverride::CurrentIndexAddress(int rowIndex) const
{
    return ModbusDriverAddress(index(rowIndex,
                                     fieldIndex(QVariant::fromValue(HAL_ADDRESS).toString())).data().toUInt());
}

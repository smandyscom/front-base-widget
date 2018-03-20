#include "tablemodeliooverride.h"

TableModelIOOverride::TableModelIOOverride(QObject *parent) : QSqlRelationalTableModel(parent)
{
    __channel = ModbusChannel::Instance();
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
    if(!(idx.column()==NAME && role==Qt::BackgroundRole))
        return QSqlRelationalTableModel::data(idx,role);

    //!Change color once the background role request comes
    if(__channel->Bit(CurrentIndexAddress(idx.row())))
        return QColor(Qt::green);
    else
        return QColor(Qt::gray);
}
//!
//! \brief TableModelIOOverride::setData
//! \param index
//! \param value
//! \param role
//! \return
//!
bool TableModelIOOverride::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //! Call base method
    if(!(index.column()==NAME && role==Qt::EditRole && __isEnableStateControl))
    {
        QSqlRelationalTableModel::setData(index,value,role);
        return true;
    }
    else
    {
        //flip the bit status
        __channel->Bit(CurrentIndexAddress(index.row()),!__channel->Bit(CurrentIndexAddress(index.row())));
    }

    return false;
}
//!
//! \brief TableModelIOOverride::CurrentIndexAddress
//! \param rowIndex
//! \return
//!
ModbusDriverAddress TableModelIOOverride::CurrentIndexAddress(int rowIndex) const
{
    return ModbusDriverAddress(index(rowIndex,HAL_ADDRESS).data().toUInt());
}

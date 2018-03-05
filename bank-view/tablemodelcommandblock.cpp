#include "tablemodelcommandblock.h"

TableModelCommandBlock::TableModelCommandBlock(QObject *parent,
                                               QSqlDatabase db):
    QSqlRelationalTableModel(parent,db)
{

}

//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \return
//! Turns row to value
ExtendedCommandBlock TableModelCommandBlock::Row(int rowIndex)
{

    QModelIndex index = index(rowIndex,0);
    ExtendedCommandBlock result;
    result.ObjectId();

    index.data();
}
//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \param value
//! Turns value to row
void TableModelCommandBlock::Row(int rowIndex, ExtendedCommandBlock value)
{

}

#include "tablemodelcommandblock.h"

//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \return
//! Turns row to value
//! Rowid should be aligned with COMMAND_BANK_ID
QVariant TableModelCommandBlock::RowRecord(int rowIndex) const
{

    __block->Bit(EXT_CTRL_BIT_0,PosCommandBlock::BIT_0_IS_ABSOLUTE,
                 __instance->record(rowIndex).value(QVariant::fromValue(EXT_CTRL_BIT_0).toString()).toBool());

    return AbstractQVariantSqlTable::RowRecord(rowIndex);
}
//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \param value
//! Turns value to row
void TableModelCommandBlock::RowRecord(int rowIndex,const QVariant value)
{
    QSqlRecord __record = record(rowIndex);
    *__block = value.value<AbstractDataBlock>();

    __record.setValue(QVariant::fromValue(EXT_CTRL_BIT_0).toString(),
                      __block->Bit(EXT_CTRL_BIT_0,PosCommandBlock::BIT_0_IS_ABSOLUTE));

    __instance->setRecord(rowIndex,__record);

    AbstractQVariantSqlTable::RowRecord(rowIndex,value);
}

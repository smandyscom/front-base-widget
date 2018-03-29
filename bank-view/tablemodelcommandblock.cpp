#include "tablemodelcommandblock.h"

TableModelCommandBlock::TableModelCommandBlock(QObject *parent):
    AbstractQVariantSqlTable(parent)
{

}

//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \return
//! Turns row to value
//! Rowid should be aligned with COMMAND_BANK_ID
QVariant TableModelCommandBlock::RowRecord(int rowIndex) const
{
    QSqlRecord __record = record(rowIndex);
    ExtendedCommandBlock result;

    //! Basic
    result.ObjectId(__record.value(QVariant::fromValue(AXIS_ID).toString()).value<MODBUS_WORD>());
    result.CommandType(BlockCommandType(__record.value(QVariant::fromValue(COMMAND_TYPE).toString()).value<MODBUS_WORD>()));
    result.Speed(__record.value(QVariant::fromValue(SPEED).toString()).toReal());
    result.Acceralation(__record.value(QVariant::fromValue(ACC_TIME).toString()).toReal());
    result.Deceralation(__record.value(QVariant::fromValue(DEC_TIME).toString()).toReal());
    result.TorqueLimit(__record.value(QVariant::fromValue(TORQUE_LIMIT).toString()).toReal());
    //! Extension
    result.ExtensionControlWord(0,__record.value(QVariant::fromValue(EXT_CTRL_BIT_0).toString()).value<bool>());
    result.ReservedWord(__record.value(QVariant::fromValue(RESERVED_WORD).toString()).value<MODBUS_WORD>());
    result.Coordinate1(__record.value(QVariant::fromValue(COORD1).toString()).toReal());
    result.Coordinate2(__record.value(QVariant::fromValue(COORD2).toString()).toReal());
    result.Coordinate3(__record.value(QVariant::fromValue(COORD3).toString()).toReal());

    return QVariant::fromValue(result);
}
//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \param value
//! Turns value to row
void TableModelCommandBlock::RowRecord(int rowIndex,const QVariant value)
{
    QSqlRecord __record = record(rowIndex);
    ExtendedCommandBlock __value = value.value<ExtendedCommandBlock>();

    //! Basic
    __record.setValue(QVariant::fromValue(AXIS_ID).toString(),QVariant::fromValue(__value.ObjectId()));
    __record.setValue(QVariant::fromValue(COMMAND_TYPE).toString(),QVariant::fromValue(static_cast<MODBUS_WORD>(__value.CommandType())));
    __record.setValue(QVariant::fromValue(SPEED).toString(),QVariant::fromValue(__value.Speed()));
    __record.setValue(QVariant::fromValue(ACC_TIME).toString(),QVariant::fromValue(__value.Acceralation()));
     __record.setValue(QVariant::fromValue(DEC_TIME).toString(),QVariant::fromValue(__value.Deceralation()));
      __record.setValue(QVariant::fromValue(TORQUE_LIMIT).toString(),QVariant::fromValue(__value.TorqueLimit()));
    //! Extension
    __record.setValue(QVariant::fromValue(EXT_CTRL_BIT_0).toString(),QVariant::fromValue(__value.ExtensionControlWord(0)));
    __record.setValue(QVariant::fromValue(RESERVED_WORD).toString(),QVariant::fromValue(__value.ReservedWord()));
    __record.setValue(QVariant::fromValue(COORD1).toString(),QVariant::fromValue(__value.Coordinate1()));
    __record.setValue(QVariant::fromValue(COORD2).toString(),QVariant::fromValue(__value.Coordinate2()));
    __record.setValue(QVariant::fromValue(COORD3).toString(),QVariant::fromValue(__value.Coordinate3()));

    setRecord(rowIndex,__record);
}

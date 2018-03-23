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

    ExtendedCommandBlock result;

    //! Basic
    result.ObjectId(index(rowIndex,AXIS_ID).data().value<MODBUS_WORD>());
    result.CommandType(BlockCommandType(index(rowIndex,COMMAND_TYPE).data().value<MODBUS_WORD>()));
    result.Speed(index(rowIndex,SPEED).data().value<float>());
    result.Acceralation(index(rowIndex,ACC_TIME).data().value<float>());
    result.Deceralation(index(rowIndex,DEC_TIME).data().value<float>());
    result.TorqueLimit(index(rowIndex,TORQUE_LIMIT).data().value<float>());
    result.ControlWord(AbstractCommandBlock::IS_RESET_POS_REFERENCE,index(rowIndex,IS_RESET_POS_R).data().value<bool>());
    //! Extension
    result.ExtensionControlWord(0,index(rowIndex,EXT_CONTROL_BIT_0).data().value<bool>());
    result.ReservedWord(index(rowIndex,RESERVED_WORD).data().value<MODBUS_WORD>());
    result.Coordinate1(index(rowIndex,COORD1).data().value<float>());
    result.Coordinate2(index(rowIndex,COORD2).data().value<float>());
    result.Coordinate3(index(rowIndex,COORD3).data().value<float>());

    return QVariant::fromValue(result);
}
//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \param value
//! Turns value to row
void TableModelCommandBlock::RowRecord(int rowIndex,const QVariant value)
{
    ExtendedCommandBlock __value = value.value<ExtendedCommandBlock>();
    //! Basic
    setData(index(rowIndex,AXIS_ID),QVariant::fromValue(__value.ObjectId()));
    setData(index(rowIndex,COMMAND_TYPE),QVariant::fromValue(static_cast<MODBUS_WORD>(__value.CommandType())));
    setData(index(rowIndex,SPEED),QVariant::fromValue(__value.Speed()));
    setData(index(rowIndex,ACC_TIME),QVariant::fromValue(__value.Acceralation()));
    setData(index(rowIndex,DEC_TIME),QVariant::fromValue(__value.Deceralation()));
    setData(index(rowIndex,TORQUE_LIMIT),QVariant::fromValue(__value.TorqueLimit()));
    setData(index(rowIndex,IS_RESET_POS_R),QVariant::fromValue(__value.ControlWord(AbstractCommandBlock::IS_RESET_POS_REFERENCE)));
    //! Extension
    setData(index(rowIndex,EXT_CONTROL_BIT_0),QVariant::fromValue(__value.ExtensionControlWord(0)));
    setData(index(rowIndex,RESERVED_WORD),QVariant::fromValue(__value.ReservedWord()));
    setData(index(rowIndex,COORD1),QVariant::fromValue(__value.Coordinate1()));
    setData(index(rowIndex,COORD2),QVariant::fromValue(__value.Coordinate2()));
    setData(index(rowIndex,COORD3),QVariant::fromValue(__value.Coordinate3()));

    submit();
}

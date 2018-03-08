#include "tablemodelcommandblock.h"

TableModelCommandBlock::TableModelCommandBlock(QSqlTableModel *instance):
    QSqlTableModel(instance->parent()),
    __instance(instance)
{
}

//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \return
//! Turns row to value
//! Rowid should be aligned with COMMAND_BANK_ID
ExtendedCommandBlock TableModelCommandBlock::Row(int rowIndex)
{

    ExtendedCommandBlock result;

    //! Basic
    result.ObjectId(__instance->index(rowIndex,AXIS_ID).data().value<MODBUS_WORD>());
    result.CommandType(BlockCommandType(__instance->index(rowIndex,COMMAND_TYPE).data().value<MODBUS_WORD>()));
    result.Speed(__instance->index(rowIndex,SPEED).data().value<float>());
    result.Acceralation(__instance->index(rowIndex,ACC_TIME).data().value<float>());
    result.Deceralation(__instance->index(rowIndex,DEC_TIME).data().value<float>());
    result.TorqueLimit(__instance->index(rowIndex,TORQUE_LIMIT).data().value<float>());
    result.IsResetPositionReference(__instance->index(rowIndex,IS_RESET_POS_R).data().value<bool>());
    //! Extension
    result.ExtensionControlWord(0,__instance->index(rowIndex,EXT_CONTROL_BIT_0).data().value<bool>());
    result.ReservedWord(__instance->index(rowIndex,RESERVED_WORD).data().value<MODBUS_WORD>());
    QVariant v = __instance->index(rowIndex,COORD1).data();
    result.Coordinate1(__instance->index(rowIndex,COORD1).data().value<float>());
    result.Coordinate2(__instance->index(rowIndex,COORD2).data().value<float>());
    result.Coordinate3(__instance->index(rowIndex,COORD3).data().value<float>());

    return result;
}
//!
//! \brief TableModelCommandBlock::Row
//! \param rowIndex
//! \param value
//! Turns value to row
void TableModelCommandBlock::Row(int rowIndex,const ExtendedCommandBlock value)
{
    //! Basic
    __instance->setData(__instance->index(rowIndex,AXIS_ID),QVariant::fromValue(value.ObjectId()));
    __instance->setData(__instance->index(rowIndex,COMMAND_TYPE),QVariant::fromValue(static_cast<MODBUS_WORD>(value.CommandType())));
    __instance->setData(__instance->index(rowIndex,SPEED),QVariant::fromValue(value.Speed()));
    __instance->setData(__instance->index(rowIndex,ACC_TIME),QVariant::fromValue(value.Acceralation()));
    __instance->setData(__instance->index(rowIndex,DEC_TIME),QVariant::fromValue(value.Deceralation()));
    __instance->setData(__instance->index(rowIndex,TORQUE_LIMIT),QVariant::fromValue(value.TorqueLimit()));
    __instance->setData(__instance->index(rowIndex,IS_RESET_POS_R),QVariant::fromValue(value.IsResetPositionReference()));
    //! Extension
    __instance->setData(__instance->index(rowIndex,EXT_CONTROL_BIT_0),QVariant::fromValue(value.ExtensionControlWord(0)));
    __instance->setData(__instance->index(rowIndex,RESERVED_WORD),QVariant::fromValue(value.ReservedWord()));
    __instance->setData(__instance->index(rowIndex,COORD1),QVariant::fromValue(value.Coordinate1()));
    __instance->setData(__instance->index(rowIndex,COORD2),QVariant::fromValue(value.Coordinate2()));
    __instance->setData(__instance->index(rowIndex,COORD3),QVariant::fromValue(value.Coordinate3()));
}

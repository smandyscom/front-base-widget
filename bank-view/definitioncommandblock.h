#ifndef DEFINITIONCOMMANDBLOCK_H
#define DEFINITIONCOMMANDBLOCK_H

#include <definitionbasicblocks.h>

namespace CommandBlock {
   Q_NAMESPACE

//!
//! \brief The DataBaseHeaders enum
//! The Header map (NAME,OFFSET VALUE)
enum DataBaseHeaders
{
    COMMAND_BLOCK_ID,
    NAME,
    COMMENT,
    //! Data
    COMMAND_TYPE = AbstractCommandBlock::OFFSET_ACB_COMMAND_TYPE,
    AXIS_ID = AbstractCommandBlock::OFFSET_ACB_AXIS_ID,
    SPEED = AbstractCommandBlock::OFFSET_ACB_SPD,
    ACC_TIME = AbstractCommandBlock::OFFSET_ACB_ACC_T,
    DEC_TIME = AbstractCommandBlock::OFFSET_ACB_DEC_T,
    TORQUE_LIMIT= AbstractCommandBlock::OFFSET_ACB_TOR_LIMIT,
    //! Data
    EXT_CTRL_BIT_0 = ExtendedCommandBlock::OFFSET_EXT_CONTROL_BIT_0,
    RESERVED_WORD = ExtendedCommandBlock::OFFSET_ECB_RESERVE_WORD,
    COORD1 = ExtendedCommandBlock::OFFSET_ECB_COORD1,
    COORD2 = ExtendedCommandBlock::OFFSET_ECB_COORD2,
    COORD3 = ExtendedCommandBlock::OFFSET_ECB_COORD3,
};
Q_ENUM_NS(Headers)

class AbstractCommandBlock : public AbstractDataBlock
{
public:
    enum Offset
    {
        OFFSET_ACB_AXIS_ID = 0,
        OFFSET_ACB_COMMAND_TYPE = 1,
        OFFSET_ACB_SPD = 2,
        OFFSET_ACB_ACC_T = 4,
        OFFSET_ACB_DEC_T = 6,
        OFFSET_ACB_TOR_LIMIT=8
    };
    enum BlockCommandType : MODBUS_WORD
    {
        BCT_NOP=0,
        BCT_POS_I=1,
        BCT_ZRET=3,
        BCT_FEED=7,
        BCT_POS_II=64,
        BCT_POS_III=65,
    };

    void Value(int key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ACB_AXIS_ID:
        case OFFSET_ACB_COMMAND_TYPE:
            Data(key,value.value<MODBUS_WORD>());
            break;
        case OFFSET_ACB_SPD:
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
        case OFFSET_ACB_TOR_LIMIT:
            Data(key,value.value<MODBUS_LONG>());
            break;
        default:
            break;
        }
    }

    QVariant Value(int key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ACB_AXIS_ID:
        case OFFSET_ACB_COMMAND_TYPE:
            return QVariant::fromValue(Data<MODBUS_WORD>(key));
        case OFFSET_ACB_SPD:
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
        case OFFSET_ACB_TOR_LIMIT:
            return QVariant::fromValue(Data<MODBUS_LONG>(key));
            break;
        default:
            return QVariant::fromValue(0);
            break;
        }
    }
};
Q_DECLARE_METATYPE(AbstractCommandBlock)
//!
//! \brief The ExtendedCommandBlock struct
//! Define the fully occupied memory for all kind of command blocks
class ExtendedCommandBlock : public AbstractCommandBlock
{
public:
    enum Offset
    {
        OFFSET_EXT_CONTROL_BIT_0,
        OFFSET_EXT_CONTROL_BIT_1,
        OFFSET_ECB_RESERVE_WORD,
        OFFSET_ECB_COORD1,
        OFFSET_ECB_COORD2,
        OFFSET_ECB_COORD3,
    };

    void Value(int index, QVariant value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_RESERVE_WORD:
            Data(index,value.value<MODBUS_WORD>());
            break;
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            Data(index,value.value<MODBUS_LONG>());
            break;
        case OFFSET_EXT_CONTROL_BIT_0:
        case OFFSET_EXT_CONTROL_BIT_1:
            Bit(index,value.toBool());
        default:
            break;
        }
    }

    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_RESERVE_WORD:
            return QVariant::fromValue(Data<MODBUS_WORD>(index));
            break;
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            return QVariant::fromValue(Data<MODBUS_LONG>(index));
            break;
        case OFFSET_EXT_CONTROL_BIT_0:
        case OFFSET_EXT_CONTROL_BIT_1:
            return Bit(index);
            break;
        default:
            break;
        }
    }

};
Q_DECLARE_METATYPE(ExtendedCommandBlock)


class PosCommandBlock : public ExtendedCommandBlock
{
public:
    enum PosIControlBit
    {
        BIT_0_ABSOLUTE_MODE = true,
        BIT_0_RELATIVE_MODE = false,
        BIT_1_CHECK_REACH = true,
        BIT_1_NO_CHECK_REACH = false,
    };
};
Q_DECLARE_METATYPE(PosCommandBlock)


class ZretCommandBlock : public ExtendedCommandBlock
{

public:
    enum ZretControlBit
    {
        //! 1:Forward/0:Backward
        BIT_0_FORWARD = true,
        BIT_0_RESERVE = false,
    };
    enum ZretMethods : MODBUS_WORD
    {
        DEC1_C_PULSE=0,
        ZERO=1,
        DEC1_ZERO =2,
        C_PULSE=3,
        C_PULSE_ONLY=11,
        POT_C_PULSE=12,
        POT_ONLY=13,
        HOME_LS_C_PULSE=14,
        HOME_ONLY=15,
        NOT_C_PULSE=16,
        NOT_ONLY=17,
        INPUT_C_PULSE=18,
        INPUT_ONLY=19,
    };
};
Q_DECLARE_METATYPE(ZretCommandBlock)

}//namespace



#endif // DEFINITIONCOMMANDBLOCK_H

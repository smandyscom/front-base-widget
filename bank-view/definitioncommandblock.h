#ifndef DEFINITIONCOMMANDBLOCK_H
#define DEFINITIONCOMMANDBLOCK_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

using namespace DEF_BASIC_DIMENSION;

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
        BCT_POS_II=64,
        BCT_POS_III=65,
    };

    void Value(int key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ACB_AXIS_ID:
        case OFFSET_ACB_COMMAND_TYPE:
            setData(key,value.value<MODBUS_WORD>());
            break;
        case OFFSET_ACB_SPD:
            setData(key,static_cast<MODBUS_LONG>(value.value<MODBUS_LONG>() * Dimension[LENGTH]));
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            setData(key,static_cast<MODBUS_LONG>(value.value<MODBUS_LONG>() * Dimension[TIME]));
            break;
        case OFFSET_ACB_TOR_LIMIT:
            setData(key,static_cast<MODBUS_LONG>(value.value<MODBUS_LONG>() * Dimension[TORQUE_RATIO]));
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
            return QVariant::fromValue(getData<MODBUS_WORD>(key));
            break;
        case OFFSET_ACB_SPD:
            return QVariant::fromValue(getData<MODBUS_LONG>(key) * Dimension[LENGTH]);
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            return QVariant::fromValue(getData<MODBUS_LONG>(key) * Dimension[TIME]);
            break;
        case OFFSET_ACB_TOR_LIMIT:
            return QVariant::fromValue(getData<MODBUS_LONG>(key) * Dimension[TORQUE_RATIO]);
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
        OFFSET_CONTROL_WORD=10,

        OFFSET_ECB_RESERVE_WORD,
        OFFSET_ECB_COORD1,
        OFFSET_ECB_COORD2,
        OFFSET_ECB_COORD3,
        //! Bit
        OFFSET_ECB_CONTROL_BIT_0 = 0x01000000+OFFSET_CONTROL_WORD,
        OFFSET_ECB_CONTROL_BIT_1 = 0x01010000+OFFSET_CONTROL_WORD,
    };
    ExtendedCommandBlock() : AbstractCommandBlock()
    {
        AbstractCommandBlock::Value(OFFSET_ACB_COMMAND_TYPE,BCT_NOP);
    }

    void Value(int index, QVariant value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_RESERVE_WORD:
            setData(index,value.value<MODBUS_WORD>());
            break;
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            setData(index,static_cast<MODBUS_LONG>(value.value<MODBUS_LONG>() * Dimension[LENGTH]));
            break;
        case OFFSET_ECB_CONTROL_BIT_0:
        case OFFSET_ECB_CONTROL_BIT_1:
            Bit(index,value.toBool());
        default:
            AbstractCommandBlock::Value(index,value);
            break;
        }
    }

    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_RESERVE_WORD:
            return QVariant::fromValue(getData<MODBUS_WORD>(index));
            break;
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            return QVariant::fromValue(getData<MODBUS_LONG>(index) * Dimension[LENGTH]);
            break;
        case OFFSET_ECB_CONTROL_BIT_0:
        case OFFSET_ECB_CONTROL_BIT_1:
            return Bit(index);
            break;
        default:
            return AbstractCommandBlock::Value(index);
            break;
        }
    }

};
Q_DECLARE_METATYPE(ExtendedCommandBlock)


class PosCommandBlock : public ExtendedCommandBlock
{
public:
    enum OffsetPos
    {
        OFFSET_POS_ABS_REL = OFFSET_ECB_CONTROL_BIT_0,
        OFFSET_POS_CHECK_REACH = OFFSET_ECB_CONTROL_BIT_1,
    };

    PosCommandBlock() : ExtendedCommandBlock()
    {
        ExtendedCommandBlock::Value(OFFSET_ACB_COMMAND_TYPE,BCT_POS_I);
    }
};
Q_DECLARE_METATYPE(PosCommandBlock)


class ZretCommandBlock : public ExtendedCommandBlock
{

public:
    enum OffsetZret
    {
        OFFSET_ZRET_DIRECTION = OFFSET_ECB_CONTROL_BIT_0,
        OFFSET_ZRET_METHOD = OFFSET_ECB_RESERVE_WORD,
        OFFSET_ZRET_SPEED=OFFSET_ACB_SPD,
        OFFSET_ZRET_SPEED_APPROACH = OFFSET_ECB_COORD2,
        OFFSET_ZRET_SPEED_CREEP = OFFSET_ECB_COORD3
    };

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

    ZretCommandBlock():ExtendedCommandBlock()
    {
        ExtendedCommandBlock::Value(OFFSET_ACB_COMMAND_TYPE,BCT_ZRET);
    }
};
Q_DECLARE_METATYPE(ZretCommandBlock)

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
    EXT_CTRL_BIT_0 = ExtendedCommandBlock::OFFSET_ECB_CONTROL_BIT_0,
    RESERVED_WORD = ExtendedCommandBlock::OFFSET_ECB_RESERVE_WORD,
    COORD1 = ExtendedCommandBlock::OFFSET_ECB_COORD1,
    COORD2 = ExtendedCommandBlock::OFFSET_ECB_COORD2,
    COORD3 = ExtendedCommandBlock::OFFSET_ECB_COORD3,
};
Q_ENUM_NS(DataBaseHeaders)

}//namespace


#endif // DEFINITIONCOMMANDBLOCK_H

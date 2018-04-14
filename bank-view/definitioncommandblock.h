#ifndef DEFINITIONCOMMANDBLOCK_H
#define DEFINITIONCOMMANDBLOCK_H

#include <definitionbasicblocks.h>

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

    void Value(int index, MODBUS_WORD value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ACB_AXIS_ID:
        case OFFSET_ACB_COMMAND_TYPE:
            AbstractCommandBlock::Value(index,value);
            break;
        default:
            break;
        }
    }
    void Value(int index, qreal value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ACB_SPD:
            AbstractCommandBlock::Value(index,value/Length());
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            AbstractCommandBlock::Value(value/Time());
            break;
        case OFFSET_ACB_TOR_LIMIT:
            AbstractCommandBlock::Value(value/TorquePercentage());
            break;
        default:
            break;
        }
    }

    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ACB_AXIS_ID:
        case OFFSET_ACB_COMMAND_TYPE:
            return QVariant::fromValue(reserved[index]);
            break;
        case OFFSET_ACB_SPD:
            return QVariant::fromValue(*reinterpret_cast<MODBUS_LONG*>(reserved[index]) * Length());
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            return QVariant::fromValue(*reinterpret_cast<MODBUS_LONG*>(reserved[index]) * Time());
            break;
        case OFFSET_ACB_TOR_LIMIT:
            return QVariant::fromValue(*reinterpret_cast<MODBUS_LONG*>(reserved[index]) * TorquePercentage());
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
        OFFSET_ECB_CONTROL_WORD,
        OFFSET_ECB_RESERVE_WORD,
        OFFSET_ECB_COORD1,
        OFFSET_ECB_COORD2,
        OFFSET_ECB_COORD3,
    };

    void Value(int index,MODBUS_WORD value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_CONTROL_WORD:
        case OFFSET_ECB_RESERVE_WORD:
            reserved[index]=value;
            break;
        default:
            break;
        }
    }
    void Value(int index, qreal value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            AbstractCommandBlock::Value(index,value/Length());
            break;
        default:
            break;
        }
    }

    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_ECB_CONTROL_WORD:
        case OFFSET_ECB_RESERVE_WORD:
            return QVariant::fromValue(reserved[index]);
            break;
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            return QVariant::fromValue(*reinterpret_cast<const MODBUS_LONG*>(&reserved[index]) * Length());
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
        BIT_0_IS_ABSOLUTE = 0x01,
        BIT_1_IS_CHECK_REACH = 0x02,
    };
};
Q_DECLARE_METATYPE(PosCommandBlock)


class ZretCommandBlock : public ExtendedCommandBlock
{

public:
    enum ZretControlBit
    {
        //! 1:Forward/0:Backward
        BIT_0_DIRECTION,
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



#endif // DEFINITIONCOMMANDBLOCK_H

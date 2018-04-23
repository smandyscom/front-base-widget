#ifndef DEFINITIONCOMMANDBLOCK_H
#define DEFINITIONCOMMANDBLOCK_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractsqltableadpater.h>

using namespace DEF_BASIC_DIMENSION;

class AbstractCommandBlock :
        public AbstractDataBlock
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
    enum CommandType
    {
        BCT_NOP=0,
        BCT_POS_I=1,
        BCT_ZRET=3,
        BCT_POS_II=64,
        BCT_POS_III=65,
    };

    void Value(uint key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ACB_SPD:
            setData(key,static_cast<MODBUS_S_LONG>(value.toReal() / Dimension->value(LENGTH)));
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            setData(key,static_cast<MODBUS_U_LONG>(value.toReal() / Dimension->value(TIME)));
            break;
        case OFFSET_ACB_TOR_LIMIT:
            setData(key,static_cast<MODBUS_U_LONG>(value.toReal() / Dimension->value(TORQUE_RATIO)));
            break;
        default:
            AbstractDataBlock::Value(key,value);
            break;
        }
    }

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ACB_SPD:
            return QVariant::fromValue(Dimension->value(LENGTH) * getData<MODBUS_S_LONG>(key));
            break;
        case OFFSET_ACB_ACC_T:
        case OFFSET_ACB_DEC_T:
            return QVariant::fromValue(Dimension->value(TIME) * getData<MODBUS_U_LONG>(key));
            break;
        case OFFSET_ACB_TOR_LIMIT:
            return QVariant::fromValue(Dimension->value(TORQUE_RATIO) * getData<MODBUS_U_LONG>(key));
            break;
        default:
            return QVariant::fromValue(key);
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
        OFFSET_CONTROL_WORD=16,

        OFFSET_ECB_RESERVE_WORD =17,
        OFFSET_ECB_COORD1 = 18,
        OFFSET_ECB_COORD2 = 20,
        OFFSET_ECB_COORD3 = 22,

    };
    enum ControlBits
    {
        //! Bit
        OFFSET_ECB_CONTROL_BIT_0=0x00000+OFFSET_CONTROL_WORD,
        OFFSET_ECB_CONTROL_BIT_1=0x10000+OFFSET_CONTROL_WORD,
    };

    ExtendedCommandBlock() : AbstractCommandBlock()
    {
        AbstractCommandBlock::Value(OFFSET_ACB_COMMAND_TYPE,BCT_NOP);
    }

    void Value(uint key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
        {
            setData(key, static_cast<MODBUS_S_LONG>(value.toReal() / Dimension->value(LENGTH)));
            break;
        }
        case OFFSET_ECB_CONTROL_BIT_0:
        case OFFSET_ECB_CONTROL_BIT_1:
            Bit(key,value.toBool());
            break;
        default:
            AbstractCommandBlock::Value(key,value);
            break;
        }
    }

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_ECB_COORD1:
        case OFFSET_ECB_COORD2:
        case OFFSET_ECB_COORD3:
            return QVariant::fromValue(getData<MODBUS_S_LONG>(key) * Dimension->value(LENGTH));
            break;
        case OFFSET_ECB_CONTROL_BIT_0:
        case OFFSET_ECB_CONTROL_BIT_1:
            return Bit(key);
            break;
        default:
            return AbstractCommandBlock::Value(key);
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
        OFFSET_ZRET_OFFSET = OFFSET_ECB_COORD1,
        OFFSET_ZRET_SPEED_APPROACH = OFFSET_ECB_COORD2,
        OFFSET_ZRET_SPEED_CREEP = OFFSET_ECB_COORD3
    };

    enum Direction
    {
        //! 1:Forward/0:Backward
        FORWARD = true,
        REVERSE = false,
    };
    enum ZretMethods
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
    ID=BASE_INDEX-1,
    NAME=BASE_INDEX-2,
    COMMENT=BASE_INDEX-3,
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

static AbstractSqlTableAdpater* Adaptor;

}//namespace


#endif // DEFINITIONCOMMANDBLOCK_H

#ifndef AXISBLOCKSDEFINITION_H
#define AXISBLOCKSDEFINITION_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractsqltableadpater.h>

using namespace DEF_BASIC_DIMENSION;

class AxisMonitorBlock :
        public AbstractDataBlock
{
    Q_OBJECT
public:
    AxisMonitorBlock(QObject* parent = nullptr) :
        AbstractDataBlock(parent){}
    AxisMonitorBlock(MODBUS_U_WORD* anchor,QObject* parent = nullptr) :
        AbstractDataBlock(anchor,parent){}

    enum MONITOR_OPERATION
    {
        MON_BIT0_SERVO_ON=0x000000,
        MON_BIT1_MACHINE_LOCK=0x010000,
        MON_BIT2_RESERVED0=0x020000,
        MON_BIT3_RESERVED1=0x030000,
        MON_BIT4_LATCH_DETECTION_REQUEST=0x040000,
        MON_BIT5_RESERVED2=0x050000,
        MON_BIT6_POSMAX_RESET=0x060000,
        MON_BIT7_ABS_INFINITE_POS_LOAD=0x070000,
        MON_BIT8_POSTIVE_EXT_TORQUE_LIMIT_INPUT=0x080000,
        MON_BIT9_NEGTIVE_EXT_TORQUE_LIMIT_INPUT=0x090000,
        MON_BIT10_RESERVERD3=0x0a0000,
        MON_BIT11_RESET_INTEGRATION=0x0b0000,
        MON_BIT12_RESET_NETWORK=0x0c0000,
        MON_BIT13_LATCH_COMPLETED_CLEAR=0x0d0000,
        MON_BIT14_RESET_COMM=0x0e0000,
        MON_BIT15_ALARM_CLEAR=0x0f0000,
    };
    Q_ENUM(MONITOR_OPERATION)
    enum WARNINGS
    {
        WARN_EXCESS_DEVIATION=0,
        WARN_SETTING_PARA_ERROR,
        WARN_FIXED_PARA_ERROR,
        WARN_SERVO_ERROR,
        WARN_MOTION_COMMAND_ERROR,
        WARN_RESERVED0,
        WARN_POT,
        WARN_NOT,
        WARN_SERVO_ON_INCOMPLETE,
        WARN_SERVO_COMM_WARNING,
        WARN_SERVO_STOP,
    };
    enum ALARMS
    {
        ALM_SERVO_ERROR=0,
        ALM_POT,
        ALM_NOT,
        ALM_PSOT,
        ALM_NSOT,
        ALM_SERVO_OFF,
        ALM_POSITION_TIMEOUT,
        ALM_EXCESS_POSITION_TRAVEL_DISTANCE,
        ALM_EXCESS_SPEED,
        ALM_EXCESS_DEVIATION,
        ALM_FILTER_TYPE_CHANGE_ERROR,
        ALM_FILTER_TIME_CONSTANT_CHANGE_ERROR,
        ALM_ZERO_POINT_UNSET,
    };
    enum COMMAND_STATUS
    {
        CS_BUSY=0,
        CS_HOLDL,
        CS_FAIL,
        CS_COMPLETE,
    };
    enum RUN_STATUS
    {
        RS_DRIVER_READY=0,
        RS_SERVO_ON,
        RS_DRIVER_BUSY,
        RS_SERVO_READY,
        RS_LATCH_MODE,
    };
    enum POS_STATUS
    {
        PS_DEN=0,
        PS_POSCOMP,
        PS_LCOMP,
        PS_NEAR,
        PS_ZERO,
        PS_ZRNC,
        PS_MLKL,
        PS_ABSLDE,
        PS_POSMAX,
    };
    enum OffsetMonitor
    {                
        OFFSET_MONITOR_TMR_ALM_COUNT_VALUE=5,
        OFFSET_MONITOR_TMR_FOCUS_COUNT_VALUE=11,
        //! Bit status
        OFFSET_MONITOR_OPERATION=0,
        OFFSET_MONITOR_RUN_STATUS=4,
        OFFSET_MONITOR_WARNINGS=6,
        OFFSET_MONITOR_ALARMS=8,
        OFFSET_MONITOR_COMMAND_STATUS=10,   
        OFFSET_MONITOR_POS_STATUS=12,
        //! Numeric
        OFFSET_MONITOR_POS_COMMAND=2,
        OFFSET_MONITOR_POS_FEEDBACK=14,
        OFFSET_MONITOR_SPD_FEEDBACK=16,
        OFFSET_MONITOR_TOR_FEEDBACK=18,
    };
    Q_ENUM(OffsetMonitor)

    //!
    //! \brief Value
    //! \param key
    //! \return
    //! Readonly
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_MONITOR_TMR_ALM_COUNT_VALUE:
        case OFFSET_MONITOR_TMR_FOCUS_COUNT_VALUE:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key) * Dimension->value(TIME));
            break;
        case OFFSET_MONITOR_WARNINGS:
        case OFFSET_MONITOR_ALARMS:
            return QVariant::fromValue(getData<MODBUS_U_LONG>(key));
            break;
        case OFFSET_MONITOR_POS_COMMAND:
        case OFFSET_MONITOR_POS_FEEDBACK:
        case OFFSET_MONITOR_SPD_FEEDBACK:
            return QVariant::fromValue(getData<MODBUS_S_LONG>(key) * Dimension->value(LENGTH));
            break;
        case OFFSET_MONITOR_TOR_FEEDBACK:
            return QVariant::fromValue(getData<MODBUS_S_LONG>(key) * Dimension->value(TORQUE_RATIO));
            break;
        default:
            return QVariant::fromValue(Bit(key));
            break;
        }
    }
};

//!
//! \brief The AxisOperationBlock class
//! Extend operation control ability
class AxisOperationBlock :
        public AxisMonitorBlock
{
    Q_OBJECT
public:
    AxisOperationBlock(QObject* parent=nullptr) :
        AxisMonitorBlock(parent){}
    AxisOperationBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AxisMonitorBlock(anchor,parent){}

    enum OffsetOperation
    {
        OFFSET_OPEATION_OPERATION=24,
    };
    Q_ENUM(OffsetOperation)
    enum OPERATION_OPERATION
    {
        OP_BIT0_SERVO_ON=0x000000 + OFFSET_OPEATION_OPERATION,
        OP_BIT1_MACHINE_LOCK=0x010000 + OFFSET_OPEATION_OPERATION,
        OP_BIT2_RESERVED0=0x020000 + OFFSET_OPEATION_OPERATION,
        OP_BIT3_RESERVED1=0x030000 + OFFSET_OPEATION_OPERATION,
        OP_BIT4_LATCH_DETECTION_REQUEST=0x040000 + OFFSET_OPEATION_OPERATION,
        OP_BIT5_RESERVED2=0x050000 + OFFSET_OPEATION_OPERATION,
        OP_BIT6_POSMAX_RESET=0x060000 + OFFSET_OPEATION_OPERATION,
        OP_BIT7_ABS_INFINITE_POS_LOAD=0x070000 + OFFSET_OPEATION_OPERATION,
        OP_BIT8_POSTIVE_EXT_TORQUE_LIMIT_INPUT=0x080000 + OFFSET_OPEATION_OPERATION,
        OP_BIT9_NEGTIVE_EXT_TORQUE_LIMIT_INPUT=0x090000 + OFFSET_OPEATION_OPERATION,
        OP_BIT10_RESERVERD3=0x0a0000 + OFFSET_OPEATION_OPERATION,
        OP_BIT11_RESET_INTEGRATION=0x0b0000 + OFFSET_OPEATION_OPERATION,
        OP_BIT12_RESET_NETWORK=0x0c0000 + OFFSET_OPEATION_OPERATION,
        OP_BIT13_LATCH_COMPLETED_CLEAR=0x0d0000 + OFFSET_OPEATION_OPERATION,
        OP_BIT14_RESET_COMM=0x0e0000 + OFFSET_OPEATION_OPERATION,
        OP_BIT15_ALARM_CLEAR=0x0f0000 + OFFSET_OPEATION_OPERATION,
    };
    Q_ENUM(OPERATION_OPERATION)

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        return AxisMonitorBlock::Value(key);
    }
    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        Bit(key,value.toBool());
    }
};

class AxisContextBlock :
        public AxisOperationBlock
{
    Q_OBJECT
public:
    AxisContextBlock(QObject* parent=nullptr) :
        AxisOperationBlock(parent){}
    AxisContextBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr):
        AxisOperationBlock(anchor,parent){}

    enum OffsetContext
    {
        OFFSET_CONTEXT_ADDRESS=32,
        OFFSET_CONTEXT_AXIS_TYPE=33,
        OFFSET_CONTEXT_LIMIT_PLUS=34,
        OFFSET_CONTEXT_LIMIT_MINUS=36,
        OFFSET_CONTEXT_SPEED_MAX=38,
        OFFSET_CONTEXT_POS_TOLERANCE=40,
    };

    void Value(uint key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_ADDRESS:
        case OFFSET_CONTEXT_AXIS_TYPE:
            setData(key,value.value<MODBUS_U_WORD>());
            break;
        case OFFSET_CONTEXT_LIMIT_PLUS:
        case OFFSET_CONTEXT_LIMIT_MINUS:
        case OFFSET_CONTEXT_SPEED_MAX:
        case OFFSET_CONTEXT_POS_TOLERANCE:
            setData(key,static_cast<MODBUS_S_LONG>(value.toReal() / Dimension->value(LENGTH)));
            break;
        default:
            AxisOperationBlock::Value(key,value);
            break;
        }
    }
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_ADDRESS:
        case OFFSET_CONTEXT_AXIS_TYPE:
            return getData<MODBUS_U_WORD>(key);
            break;
        case OFFSET_CONTEXT_LIMIT_PLUS:
        case OFFSET_CONTEXT_LIMIT_MINUS:
        case OFFSET_CONTEXT_SPEED_MAX:
        case OFFSET_CONTEXT_POS_TOLERANCE:
            return QVariant::fromValue(getData<MODBUS_S_LONG>(key) * Dimension->value(LENGTH));
            break;
        default:
            return AxisMonitorBlock::Value(key);
            break;
        }
    }

};

namespace AxisBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID=INVALID_INDEX-1,
    REGION=INVALID_INDEX-2,
    NAME=INVALID_INDEX-3,
    COMMENT=INVALID_INDEX-4,
    //! Data
    ADDRESS = AxisContextBlock::OFFSET_CONTEXT_ADDRESS,
    TYPE = AxisContextBlock::OFFSET_CONTEXT_AXIS_TYPE,
    LIMIT_MINUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_MINUS,
    LIMIT_PLUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_PLUS,
    POSITION_TOLERANCE = AxisContextBlock::OFFSET_CONTEXT_POS_TOLERANCE,
    SPEED_MAX = AxisContextBlock::OFFSET_CONTEXT_SPEED_MAX,
};
Q_ENUM_NS(DataBaseHeaders)

}


#endif // AXISBLOCKSDEFINITION_H

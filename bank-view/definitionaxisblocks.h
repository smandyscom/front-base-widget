#ifndef AXISBLOCKSDEFINITION_H
#define AXISBLOCKSDEFINITION_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractsqltableadpater.h>

using namespace DEF_BASIC_DIMENSION;

class AxisMonitorBlock :
        public AbstractDataBlock
{
public:
    enum OPERATION
    {
        OP_SERVO_ON=0,
        OP_MACHINE_LOCK,
        OP_RESERVED0,
        OP_RESERVED1,
        OP_LATCH_DETECTION_REQUEST,
        OP_RESERVED2,
        OP_POSMAX_RESET,
        OP_ABS_INFINITE_POS_LOAD,
        OP_POSTIVE_EXT_TORQUE_LIMIT_INPUT,
        OP_NEGTIVE_EXT_TORQUE_LIMIT_INPUT,
        OP_RESERVERD3,
        OP_RESET_INTEGRATION,
        OP_RESET_NETWORK,
        OP_LATCH_COMPLETED_CLEAR,
        OP_RESET_COMM,
        OP_ALARM_CLEAR,
    };
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
        RS_SERVO_READY=0,
        RS_SERVO_ON,
        RS_SYS_BUSY,
        RS_SERVO_BUSY,
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
Q_DECLARE_METATYPE(AxisMonitorBlock)

//!
//! \brief The AxisOperationBlock class
//! Extend operation control ability
class AxisOperationBlock : public AxisMonitorBlock
{
public:
    enum OffsetOperation
    {
        OFFSET_OPEATION_OPERATION=24,
    };
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        return AxisMonitorBlock::Value(key);
    }
    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        Bit(key,value.toBool());
    }
};
Q_DECLARE_METATYPE(AxisOperationBlock)

class AxisContextBlock : public AxisOperationBlock
{

public:
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
Q_DECLARE_METATYPE(AxisContextBlock)

namespace AxisBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID=BASE_INDEX-1,
    REGION=BASE_INDEX-2,
    NAME=BASE_INDEX-3,
    COMMENT=BASE_INDEX-4,
    //! Data
    ADDRESS = AxisContextBlock::OFFSET_CONTEXT_ADDRESS,
    TYPE = AxisContextBlock::OFFSET_CONTEXT_AXIS_TYPE,
    LIMIT_MINUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_MINUS,
    LIMIT_PLUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_PLUS,
    POSITION_TOLERANCE = AxisContextBlock::OFFSET_CONTEXT_POS_TOLERANCE,
    SPEED_MAX = AxisContextBlock::OFFSET_CONTEXT_SPEED_MAX,
};
Q_ENUM_NS(DataBaseHeaders)

//!
//! \brief Adaptor
//! Initialize after data base initialized
static AbstractSqlTableAdpater* Adaptor = nullptr;
}


#endif // AXISBLOCKSDEFINITION_H

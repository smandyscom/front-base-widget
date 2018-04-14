#ifndef AXISBLOCKSDEFINITION_H
#define AXISBLOCKSDEFINITION_H

#include <definitionbasicblocks.h>

class AxisMonitorBlock : public AbstractDataBlock
{
public:
    enum OPERATION : quint16
    {
        OP_SERVO_ON=0x0001,
        OP_MACHINE_LOCK=0x0002,
        OP_RESERVED0=0x0004,
        OP_RESERVED1=0x0008,
        OP_LATCH_DETECTION_REQUEST=0x0010,
        OP_RESERVED2=0x0020,
        OP_POSMAX_RESET=0x0040,
        OP_ABS_INFINITE_POS_LOAD=0x0080,
        OP_POSTIVE_EXT_TORQUE_LIMIT_INPUT=0x0100,
        OP_NEGTIVE_EXT_TORQUE_LIMIT_INPUT=0x0200,
        OP_RESERVERD3=0x0400,
        OP_RESET_INTEGRATION=0x0800,
        OP_RESET_NETWORK=0x1000,
        OP_LATCH_COMPLETED_CLEAR=0x2000,
        OP_RESET_COMM=0x4000,
        OP_ALARM_CLEAR=0x8000,
    };
    enum WARNINGS : MODBUS_LONG
    {
        WARN_EXCESS_DEVIATION=0x0001,
        WARN_SETTING_PARA_ERROR=0x0002,
        WARN_FIXED_PARA_ERROR=0x0004,
        WARN_SERVO_ERROR=0x0008,
        WARN_MOTION_COMMAND_ERROR=0x0010,
        WARN_RESERVED0=0x0020,
        WARN_POT=0x0040,
        WARN_NOT=0x0080,
        WARN_SERVO_ON_INCOMPLETE=0x0100,
        WARN_SERVO_COMM_WARNING=0x0200,
        WARN_SERVO_STOP=0x0400,
    };
    enum ALARMS : MODBUS_LONG
    {
        ALM_SERVO_ERROR=0x0001,
        ALM_POT=0x0002,
        ALM_NOT=0x0004,
        ALM_PSOT=0x0008,
        ALM_NSOT=0x0010,
        ALM_SERVO_OFF=0x0020,
        ALM_POSITION_TIMEOUT=0x0040,
        ALM_EXCESS_POSITION_TRAVEL_DISTANCE=0x0080,
        ALM_EXCESS_SPEED=0x0100,
        ALM_EXCESS_DEVIATION=0x0200,
        ALM_FILTER_TYPE_CHANGE_ERROR=0x0400,
        ALM_FILTER_TIME_CONSTANT_CHANGE_ERROR=0x0800,
        ALM_ZERO_POINT_UNSET=0x2000,
    };
    enum COMMAND_STATUS : MODBUS_WORD
    {
        CS_BUSY=0x0001,
        CS_HOLDL=0x0002,
        CS_FAIL=0x0008,
        CS_COMPLETE=0x0100,
    };
    enum RUN_STATUS : MODBUS_WORD
    {
        RS_SERVO_READY=0x0001,
        RS_SERVO_ON=0x0002,
        RS_SYS_BUSY=0x0004,
        RS_SERVO_BUSY=0x0008,
        RS_LATCH_MODE=0x0010,
    };
    enum POS_STATUS : MODBUS_WORD
    {
        PS_DEN=0x0001,
        PS_POSCOMP=0x0002,
        PS_LCOMP=0x0004,
        PS_NEAR=0x0008,
        PS_ZERO=0x0010,
        PS_ZRNC=0x0020,
        PS_MLKL=0x0040,
        PS_ABSLDE=0x0100,
        PS_POSMAX=0x0200,
    };
    enum OffsetMonitor
    {
        OFFSET_MONITOR_OPERATION=0,
        OFFSET_MONITOR_POS_COMMAND=2,
        OFFSET_MONITOR_RUN_STATUS=4,
        OFFSET_MONITOR_TMR_ALM_COUNT_VALUE=5,
        OFFSET_MONITOR_WARNINGS=6,
        OFFSET_MONITOR_ALARMS=8,
        OFFSET_MONITOR_COMMAND_STATUS=10,
        OFFSET_MONITOR_TMR_FOCUS_COUNT_VALUE=11,
        OFFSET_MONITOR_POS_STATUS=12,
        OFFSET_MONITOR_POS_FEEDBACK=14,
        OFFSET_MONITOR_SPD_FEEDBACK=16,
        OFFSET_MONITOR_TOR_FEEDBACK=18,
    };

    //!
    //! \brief Operation
    //! \param value
    //! \return
    //!
    bool Operation(OPERATION value) const
    {
        return (reserved[OFFSET_MONITOR_OPERATION] & value) > 0;
    }
    //!
    //! \brief positionCommand
    //! in 0.001mm
    qreal PositionCommand() const { return *reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_POS_COMMAND]) * Length();}
    bool RunStatus(RUN_STATUS value)
    {
        return (reserved[OFFSET_MONITOR_RUN_STATUS] & value) > 0;
    }
    WARNINGS Warning() const
    {
        return WARNINGS(*reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_WARNINGS]));
    }
    ALARMS Alarm() const
    {
        return ALARMS(*reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_ALARMS]));
    }
    bool CommandStatus(COMMAND_STATUS value) const
    {
        return (reserved[OFFSET_MONITOR_COMMAND_STATUS] & value) > 0;
    }
    bool PositionStatus(POS_STATUS value) const
    {
        return (reserved[OFFSET_MONITOR_POS_STATUS] & value) > 0;
    }


    //!
    //! \brief positionFeedback
    //! in 0.001mm
    qreal PositionFeedback() const {return *reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_POS_FEEDBACK]) * Length();}
    //!
    //! \brief speedFeedback
    //! in 0.001mm/sec
    qreal SpeedFeedback() const {return *reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_SPD_FEEDBACK]) * Length();}
    //!
    //! \brief torqueFeedback
    //! in 0.01%
    qreal TorqueFeedback() const {return *reinterpret_cast<const MODBUS_LONG*>(&reserved[OFFSET_MONITOR_TOR_FEEDBACK]) * TorquePercentage();}
};
Q_DECLARE_METATYPE(AxisMonitorBlock)

//!
//! \brief The AxisOperationBlock class
//! Extend operation control ability
class AxisOperationBlock : public AxisMonitorBlock
{
    enum OffsetOperation
    {
        OFFSET_OPEATION_OPERATION=24,
    };

public:
    void Operation(OPERATION bit,bool value)
    {
        if(value)
            reserved[OFFSET_OPEATION_OPERATION] |= bit;
        else
            reserved[OFFSET_OPEATION_OPERATION] &= ~bit;
    }
};
Q_DECLARE_METATYPE(AxisOperationBlock)

class AxisContextBlock : public AxisMonitorBlock
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

    void MotionParameter(OffsetContext itemIndex,MODBUS_WORD value)
    {
        reserved[itemIndex] = value;
    }
    void MotionParameter(OffsetContext itemIndex,qreal value)
    {
        *(reinterpret_cast<MODBUS_LONG*>(&(reserved[itemIndex]))) = value / Length();
    }
    qreal MotionParameter(OffsetContext itemIndex) const
    {
        switch (itemIndex) {
        case OFFSET_CONTEXT_ADDRESS:
        case OFFSET_CONTEXT_AXIS_TYPE:
            return reserved[itemIndex];
            break;
        default:
            return *(reinterpret_cast<const MODBUS_LONG*>(&reserved[itemIndex]))*Length();
            break;
        }


    }
};
Q_DECLARE_METATYPE(AxisContextBlock)

#endif // AXISBLOCKSDEFINITION_H

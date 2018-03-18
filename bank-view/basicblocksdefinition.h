#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H
#include <baselayerdefinitions.h>
using namespace BaseLayer;

#pragma pack(1)

enum BlockCommandType : MODBUS_WORD
{
    BCT_NOP=0,
    BCT_POS_I=1,
    BCT_ZRET=3,
    BCT_FEED=7,
    BCT_POS_II=64,
    BCT_POS_III=65,
};

class AbstractCommandBlock
{
public:
    enum ControlBit
    {
        IS_PARA_SETTED,
        IS_RESET_POS_REFERENCE,
    };

    void ObjectId(MODBUS_WORD value)
    {
        __objectId = value;
    }
    MODBUS_WORD ObjectId() const
    {
        return __objectId;
    }
    void CommandType(BlockCommandType value)
    {
        __commandType = value;
    }
    BlockCommandType CommandType() const
    {
        return __commandType;
    }
    void Speed(qreal value)
    {
        __speed = value / Length();
    }
    qreal Speed() const
    {
        return __speed * Length();
    }
    void Acceralation(qreal value)
    {
        __acceralation = value / Time();
    }
    qreal Acceralation() const
    {
        return __acceralation * Time();
    }
    void Deceralation(qreal value)
    {
        __deceralation = value / Time();
    }
    qreal Deceralation() const
    {
        return __deceralation * Time();
    }
    void TorqueLimit(qreal value)
    {
        __torqueLimit = value / TorquePercentage();
    }
    qreal TorqueLimit() const
    {
        return __torqueLimit * TorquePercentage();
    }

    void ControlWord(int bitIndex,bool value)
    {
        if(value)
            __controlWord |= (0x01 << bitIndex);
        else
            __controlWord &= ~(0x01 << bitIndex);
    }
    bool ControlWord(int bitIndex) const
    {
        return (__controlWord & (0x01 << bitIndex)) > 0;
    }
protected:
    MODBUS_WORD __objectId;
    BlockCommandType __commandType;
    MODBUS_LONG __speed;
    MODBUS_LONG __acceralation;
    MODBUS_LONG __deceralation;
    MODBUS_LONG __torqueLimit;
    MODBUS_WORD __controlWord; //#10
    MODBUS_WORD __reserved[5];//#11-#15
    //!
    //! \brief Length
    //! in mm
    //! TODOS , query axis parameter table , get right ratio
    qreal Length() const
    {
        return 0.001;
    }
    //!
    //! \brief Time
    //! in sec
    qreal Time() const
    {
        return 0.001;
    }
    //!
    //! \brief Torque
    //! \return
    //! in Percentage
    qreal TorquePercentage() const
    {
        return 0.01;
    }


};
Q_DECLARE_METATYPE(AbstractCommandBlock)
//!
//! \brief The ExtendedCommandBlock struct
//! Define the fully occupied memory for all kind of command blocks
class ExtendedCommandBlock : public AbstractCommandBlock
{
public:
    //!
    //! \brief setCoordinate
    //! \param coordinateInMm
    //!
    void Coordinate1(qreal value){ __coord1Offset = value / Length();}
    qreal Coordinate1() const{ return __coord1Offset * Length();}

    void Coordinate2(qreal value) { __coord2SpeedCreep = value/Length();}
    qreal Coordinate2() const {return __coord2SpeedCreep * Length();}

    void Coordinate3(qreal value) { __coord3SpeedApproach = value / Length();}
    qreal Coordinate3() const {return __coord3SpeedApproach * Length();}

    //!
    //! \brief ExtensionControlWord
    //! \param bitIndex
    //! \param value
    //! definition would be varied by different child
    void ExtensionControlWord(int bitIndex,bool value)
    {
        if(value)
            __extendControlWord |= (0x01 << bitIndex);
        else
            __extendControlWord &= ~(0x01 << bitIndex);
    }
    bool ExtensionControlWord(int bitIndex) const
    {
        return (__extendControlWord & (0x01 << bitIndex)) > 0;
    }
    void ReservedWord(MODBUS_WORD value)
    {
        __reservedWord = value;
    }
    MODBUS_WORD ReservedWord() const
    {
        return __reservedWord;
    }

protected:
    MODBUS_WORD __extendControlWord;
    MODBUS_WORD __reservedWord;
    MODBUS_LONG __coord1Offset;
    MODBUS_LONG __coord2SpeedCreep;
    MODBUS_LONG __coord3SpeedApproach;
};
Q_DECLARE_METATYPE(ExtendedCommandBlock)
class PosICommandBlock : public ExtendedCommandBlock
{
protected:
    enum ExtendControlBit
    {
        BIT_0_IS_ABSOLUTE,
        BIT_1_IS_CHECK_REACH,
    };
public:

    void IsAbsoluteMode(bool value)
    {
        ExtensionControlWord(BIT_0_IS_ABSOLUTE,value);
    }
    bool IsAbsoluteMode() const
    {
        return ExtendControlBit(BIT_0_IS_ABSOLUTE);
    }
    void IsCheckReach(bool value)
    {
        ExtensionControlWord(BIT_1_IS_CHECK_REACH,value);
    }
    bool IsCheckReach() const
    {
        return ExtensionControlWord(BIT_1_IS_CHECK_REACH);
    }  
};
Q_DECLARE_METATYPE(PosICommandBlock)

enum ZretMethods : MODBUS_WORD
{
    DEC1_C_PULSE=0,
};
class ZretCommandBlock : public ExtendedCommandBlock
{
protected:
    enum ExtendControlBit
    {
        //! 1:Forward/0:Backward
        BIT_0_DIRECTION,
    };
public:

    //!
    //! \brief Direction
    //! \param isForward
    //! True : forward
    void Direction(bool value)
    {
        ExtensionControlWord(BIT_0_DIRECTION,value);
    }
    bool Direction() const
    {
        return ExtensionControlWord(BIT_0_DIRECTION);
    }
    void Method(ZretMethods value)
    {
        ReservedWord(value);
    }
    ZretMethods Method() const
    {
        return ZretMethods(ReservedWord());
    }
    void Offset(qreal value)
    {
        Coordinate1(value);
    }
    qreal Offset() const
    {
        return Coordinate1();
    }
    void SpeedCreep(qreal value)
    {
        Coordinate2(value);
    }
    qreal SpeedCreep() const
    {
        return Coordinate2();
    }
    void SpeedAppoach(qreal value)
    {
        Coordinate3(value);
    }
    qreal SpeedAppoach() const
    {
        return Coordinate3();
    }

};
Q_DECLARE_METATYPE(ZretCommandBlock)
class FeedCommandBlock : public ExtendedCommandBlock
{
protected:
    enum ExtendControlBit
    {
        //! 1:Forward/0:Backward
        BIT_0_DIRECTION,
    };
public:

    void Direction(bool value)
    {
        ExtensionControlWord(BIT_0_DIRECTION,value);
    }
    bool Direction() const
    {
        return ExtensionControlWord(BIT_0_DIRECTION);
    }
};
Q_DECLARE_METATYPE(FeedCommandBlock)


typedef MODBUS_LONG CommitIndex ;

//!
//! \brief The CommitBlock class
//!
class CommitBlock
{
public:
    enum CommitSelection : MODBUS_LONG
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_COMMAND_BLOCK = 2,
    };
    enum CommitMode : MODBUS_LONG
    {
        MODE_COMMAND_BLOCK=0,
        MODE_DOWNLOAD = 15, //PLC<-HMI
        MODE_UPLOAD = 16,   //PLC->HMI
    };
    void Mode(CommitMode value) {__mode = value;}
    CommitMode Mode() const {return __mode;}
    void Selection(CommitSelection value) {__selection = value;}
    CommitSelection Selection() const {return __selection;}
    void Index(CommitIndex value) {__index = value;}
    CommitIndex Index() const {return __index;}
protected:
    CommitMode __mode;
    CommitSelection __selection;
    CommitIndex __index;
};
Q_DECLARE_METATYPE(CommitBlock)

#define MONITOR_BLOCK_FULL_OCCUPATION 24
class AbstractMonitorBlock
{
protected:
    MODBUS_WORD reserved[MONITOR_BLOCK_FULL_OCCUPATION];

    //!
    //! \brief Length
    //! in mm
    //! TODOS , query axis parameter table , get right ratio
    qreal Length() const
    {
        return 0.001;
    }
    //!
    //! \brief Time
    //! in sec
    qreal Time() const
    {
        return 0.001;
    }
    //!
    //! \brief Torque
    //! \return
    //! in Percentage
    qreal TorquePercentage() const
    {
        return 0.01;
    }

};
Q_DECLARE_METATYPE(AbstractMonitorBlock)
class AxisMonitorBlock : public AbstractMonitorBlock
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

    //!
    //! \brief Operation
    //! \param value
    //! \return
    //!
    bool Operation(OPERATION value) const
    {
        return (reserved[0] & value) > 0;
    }
    //!
    //! \brief positionCommand
    //! in 0.001mm
    qreal PositionCommand() const { return reinterpret_cast<const MODBUS_LONG*>(reserved)[1] * Length();}
    bool RunStatus(RUN_STATUS value)
    {
        return (reserved[4] & value) > 0;
    }
    WARNINGS Warning() const
    {
        return WARNINGS(reinterpret_cast<const MODBUS_LONG*>(reserved)[3]);
    }
    ALARMS Alarm() const
    {
        return ALARMS(reinterpret_cast<const MODBUS_LONG*>(reserved)[4]);
    }
    bool CommandStatus(COMMAND_STATUS value) const
    {
        return (reserved[10] & value) > 0;
    }
    bool PositionStatus(POS_STATUS value) const
    {
        return (reserved[12] & value) > 0;
    }


    //!
    //! \brief positionFeedback
    //! in 0.001mm
    qreal PositionFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[7] * Length();}
    //!
    //! \brief speedFeedback
    //! in 0.001mm/sec
    qreal SpeedFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[8] * Length();}
    //!
    //! \brief torqueFeedback
    //! in 0.01%
    qreal TorqueFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[9] * TorquePercentage();}
};
Q_DECLARE_METATYPE(AxisMonitorBlock)


#endif // BASICBLOCKSDEFINITION_H

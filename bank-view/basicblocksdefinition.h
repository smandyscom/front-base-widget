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
protected:
    enum ControlBit
    {
        IS_PARA_SETTED,
        IS_RESET_POS_REFERENCE,
    };
public:
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
    void IsResetPositionReference(bool isReset)
    {
        if(isReset)
            __controlWord |= 0x01;
        else
            __controlWord &= ~0x01;
    }
    bool IsResetPositionReference() const
    {
        return (__controlWord & 0x01) > 0;
    }
    void Speed(float value)
    {
        __speed = value / Length();
    }
    float Speed() const
    {
        return __speed * Length();
    }
    void Acceralation(float value)
    {
        __acceralation = value / Time();
    }
    float Acceralation() const
    {
        return __acceralation * Time();
    }
    void Deceralation(float value)
    {
        __deceralation = value / Time();
    }
    float Deceralation() const
    {
        return __deceralation * Time();
    }
    void TorqueLimit(float value)
    {
        __torqueLimit = value / TorquePercentage();
    }
    float TorqueLimit() const
    {
        return __torqueLimit * TorquePercentage();
    }
protected:
    MODBUS_WORD __objectId;
    BlockCommandType __commandType;
    MODBUS_LONG __speed;
    MODBUS_LONG __acceralation;
    MODBUS_LONG __deceralation;
    MODBUS_LONG __torqueLimit;
    MODBUS_WORD __controlWord;

    //!
    //! \brief Length
    //! in mm
    //! TODOS , query axis parameter table , get right ratio
    float Length() const
    {
        return 0.001;
    }
    //!
    //! \brief Time
    //! in sec
    float Time() const
    {
        return 0.001;
    }
    //!
    //! \brief Torque
    //! \return
    //! in Percentage
    float TorquePercentage() const
    {
        return 0.01;
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
    void Coordinate1(float value){ __coord1Offset = value / Length();}
    float Coordinate1() const{ return __coord1Offset * Length();}

    void Coordinate2(float value) { __coord2SpeedCreep = value/Length();}
    float Coordinate2() const {return __coord2SpeedCreep * Length();}

    void Coordinate3(float value) { __coord3SpeedApproach = value / Length();}
    float Coordinate3() const {return __coord3SpeedApproach * Length();}

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
    void Offset(float value)
    {
        Coordinate1(value);
    }
    float Offset() const
    {
        return Coordinate1();
    }
    void SpeedCreep(float value)
    {
        Coordinate2(value);
    }
    float SpeedCreep() const
    {
        return Coordinate2();
    }
    void SpeedAppoach(float value)
    {
        Coordinate3(value);
    }
    float SpeedAppoach() const
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


typedef quint16 CommitIndex ;

//!
//! \brief The CommitBlock class
//!
class CommitBlock
{
public:
    enum CommitSelection : MODBUS_WORD
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_COMMAND_BLOCK = 2,
    };
    enum CommitMode : MODBUS_WORD
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

#define MONITOR_BLOCK_FULL_OCCUPATION 10
class AbstractMonitorBlock
{
protected:
    MODBUS_WORD reserved[MONITOR_BLOCK_FULL_OCCUPATION];

    //!
    //! \brief Length
    //! in mm
    //! TODOS , query axis parameter table , get right ratio
    float Length() const
    {
        return 0.001;
    }
    //!
    //! \brief Time
    //! in sec
    float Time() const
    {
        return 0.001;
    }
    //!
    //! \brief Torque
    //! \return
    //! in Percentage
    float TorquePercentage() const
    {
        return 0.01;
    }

};
Q_DECLARE_METATYPE(AbstractMonitorBlock)
class AxisMonitorBlock : public AbstractMonitorBlock
{
public:
    //!
    //! \brief positionCommand
    //! in 0.001mm
    float PositionCommand() const { return reinterpret_cast<const MODBUS_LONG*>(reserved)[0] * Length();}

    //!
    //! \brief positionFeedback
    //! in 0.001mm
    float PositionFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[1] * Length();}
    //!
    //! \brief speedFeedback
    //! in 0.001mm/sec
    float SpeedFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[2] * Length();}
    //!
    //! \brief torqueFeedback
    //! in 0.01%
    float TorqueFeedback() const {return reinterpret_cast<const MODBUS_LONG*>(reserved)[3] * TorquePercentage();}
};
Q_DECLARE_METATYPE(AxisMonitorBlock)


#endif // BASICBLOCKSDEFINITION_H

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
    { return 0.01;
    }


};
Q_DECLARE_METATYPE(AbstractCommandBlock)
//!
//! \brief The ExtendedCommandBlock struct
//! Define the fully occupied memory for all kind of command blocks
class ExtendedCommandBlock : public AbstractCommandBlock
{
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
public:
    void IsAbsoluteMode(bool isAbsolute)
    {
        if(isAbsolute)
            __extendControlWord |= 0x01;
        else
            __extendControlWord &= ~0x01;
    }
    bool IsAbsoluteMode() const
    {
        return (__extendControlWord & 0x01) > 0;
    }
    void IsCheckReach(bool isCheck)
    {
        if(isCheck)
            __extendControlWord |= 0x02;
        else
            __extendControlWord &= ~0x02;
    }
    bool IsCheckReach() const
    {
        return (__extendControlWord & 0x02) > 0;
    }

    //!
    //! \brief setCoordinate
    //! \param coordinateInMm
    //!
    void Coordinate(float value)
    {
        __coord1Offset = value / Length();
    }
    float Coordinate() const
    {
        return __coord1Offset * Length();
    }
};
Q_DECLARE_METATYPE(PosICommandBlock)

enum ZretMethods : MODBUS_WORD
{
    DEC1_C_PULSE=0,
};
class ZretCommandBlock : public ExtendedCommandBlock
{
public:
    //!
    //! \brief Direction
    //! \param isForward
    //! True : forward
    void Direction(bool isForward)
    {
        if(isForward)
            __extendControlWord |= 0x01;
        else
            __extendControlWord &= ~0x01;
    }
    bool Direction() const
    {
        return (__extendControlWord & 0x01) > 0;
    }
    void Method(ZretMethods value)
    {
        __reservedWord = value;
    }
    ZretMethods Method() const
    {
        return ZretMethods(__reservedWord);
    }
    void Offset(float value)
    {
        __coord1Offset = value / Length();
    }
    float Offset() const
    {
        return __coord1Offset * Length();
    }
    void SpeedCreep(float value)
    {
        __coord2SpeedCreep = value / Length();
    }
    float SpeedCreep() const
    {
        return __coord2SpeedCreep * Length();
    }
    void SpeedAppoach(float value)
    {
        __coord3SpeedApproach = value / Length();
    }
    float SpeedAppoach() const
    {
        return __coord3SpeedApproach * Length();
    }

};
Q_DECLARE_METATYPE(ZretCommandBlock)
class FeedCommandBlock : public ExtendedCommandBlock
{
public:
    void Direction(bool isForward)
    {
        if(isForward)
            __extendControlWord |= 0x01;
        else
            __extendControlWord &= ~0x01;
    }
    bool Direction() const
    {
        return (__extendControlWord & 0x01) > 0;
    }
};
Q_DECLARE_METATYPE(FeedCommandBlock)


typedef quint16 CommitIndex ;


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

#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H
#include <baselayerdefinitions.h>
using namespace BaseLayer;

#pragma pack(1)
#define DATA_BLOCK_SIZE_IN_WORD 64
class AbstractDataBlock
{
public:
    AbstractDataBlock()
    {
        memset(reserved,0,sizeof(MODBUS_WORD) * DATA_BLOCK_SIZE_IN_WORD);
    }

    void Value(int index,MODBUS_WORD value)
    {
        reserved[index] = value;
    }
    MODBUS_WORD Value(int index) const
    {
        return reserved[index];
    }
protected:
    MODBUS_WORD reserved[DATA_BLOCK_SIZE_IN_WORD];
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
Q_DECLARE_METATYPE(AbstractDataBlock)

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


typedef MODBUS_LONG CommitDataBlockIndex ;

//!
//! \brief The CommitBlock class
//!
class CommitBlock
{
public:
    //!
    //! \brief The CommitSelection enum
    //! Data block selection
    enum CommitDataBlockSelection : MODBUS_LONG
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_COMMAND_BLOCK = 2,
        SELECTION_UNIT = 3,
    };
    enum CommitMode : MODBUS_LONG
    {
        MODE_EXE_COMMAND_BLOCK=0,
        MODE_EXE_CYLINDER=1,
        MODE_EXE_AXIS=8,
        MODE_DOWNLOAD_DATA_BLOCK = 15, //PLC<-HMI
        MODE_UPLOAD_DATA_BLOCK = 16,   //PLC->HMI
    };
    CommitBlock()
    {
        __mode = MODE_EXE_COMMAND_BLOCK;
        __selection = SELECTION_COMMAND_BLOCK;
        __index = 0;
    }

    void Mode(CommitMode value) {__mode = value;}
    CommitMode Mode() const {return __mode;}
    void Selection(CommitDataBlockSelection value) {__selection = value;}
    CommitDataBlockSelection Selection() const {return __selection;}
    void Index(CommitDataBlockIndex value) {__index = value;}
    CommitDataBlockIndex Index() const {return __index;}
protected:
    CommitMode __mode;
    CommitDataBlockSelection __selection;
    CommitDataBlockIndex __index;
};
Q_DECLARE_METATYPE(CommitBlock)



class IoMonitorOverrideBlock
{
public:
protected:
    MODBUS_WORD reserved[8];
};
Q_DECLARE_METATYPE(IoMonitorOverrideBlock)



#endif // BASICBLOCKSDEFINITION_H

#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H
#include <baselayerdefinitions.h>
using namespace BaseLayer;

#pragma pack(1)

#define COMMAND_BLOCK_FULL_OCCUPATION 96

enum BlockCommandType : MODBUS_WORD
{
    NOP=0,
    POS_I=1,
    ZRET=3,
    FEED=7,
    POS_II=64,
    POS_III=65,
};

struct AbstractCommandBlock
{
    MODBUS_WORD objectId;
    BlockCommandType commandType;
    MODBUS_LONG speed;
    MODBUS_LONG acceralation;
    MODBUS_LONG deceralation;
    MODBUS_LONG torqueLimit;
    MODBUS_WORD controlWord;
};
Q_DECLARE_METATYPE(AbstractCommandBlock)


struct PosICommandBlock : AbstractCommandBlock
{
    MODBUS_LONG extendControlWord;
    MODBUS_LONG coordinate;

    void setAbsoluteMode(bool isAbsolute)
    {
        if(isAbsolute)
            extendControlWord |= 0x01;
        else
            extendControlWord &= ~0x01;
    }
};
Q_DECLARE_METATYPE(PosICommandBlock)

enum ZretMethods : MODBUS_WORD
{
    DEC1_C_PULSE=0,
};
struct ZretCommandBlock : AbstractCommandBlock
{
    MODBUS_WORD extendControlWord;
    ZretMethods method;
    MODBUS_LONG offset;
    MODBUS_LONG speedCreep;
    MODBUS_LONG speedAppoach;

    void setDirection(bool isForward)
    {
        if(isForward)
            extendControlWord |= 0x01;
        else
            extendControlWord &= ~0x01;
    }
};
Q_DECLARE_METATYPE(ZretCommandBlock)
struct FeedCommandBlock : AbstractCommandBlock
{
    MODBUS_WORD extendControlWord;

    void setDirection(bool isForward)
    {
        if(isForward)
            extendControlWord |= 0x01;
        else
            extendControlWord &= ~0x01;
    }
};
Q_DECLARE_METATYPE(FeedCommandBlock)
struct GenericCommandBlock
{
    MODBUS_WORD reserved[COMMAND_BLOCK_FULL_OCCUPATION];
};
Q_DECLARE_METATYPE(GenericCommandBlock)



#endif // BASICBLOCKSDEFINITION_H

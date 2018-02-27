#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H
#include <baselayerdefinitions.h>
using namespace BaseLayer;

#pragma pack(1)

#define COMMAND_BLOCK_FULL_OCCUPATION 96

enum BlockCommandType : WORD
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
    WORD objectId;
    BlockCommandType commandType;
    LONG speed;
    LONG acceralation;
    LONG deceralation;
    LONG torqueLimit;
    WORD controlWord;
};
Q_DECLARE_METATYPE(AbstractCommandBlock)


struct PosICommandBlock : AbstractCommandBlock
{
    LONG extendControlWord;
    LONG coordinate;
};
Q_DECLARE_METATYPE(PosICommandBlock)

enum ZretMethods : WORD
{
    DEC1_C_PULSE=0,
};
struct ZretCommandBlock : AbstractCommandBlock
{
    WORD extendControlWord;
    ZretMethods method;
    LONG offset;
    LONG speedCreep;
    LONG speedAppoach;
};
Q_DECLARE_METATYPE(ZretCommandBlock)
struct FullCommandBlock
{
    WORD reserved[COMMAND_BLOCK_FULL_OCCUPATION];
};
Q_DECLARE_METATYPE(FullCommandBlock)



#endif // BASICBLOCKSDEFINITION_H

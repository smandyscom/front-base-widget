#ifndef DEFINITIONSLOTBLOCK_H
#define DEFINITIONSLOTBLOCK_H

#include <definitionbasicblocks.h>

//!
//! \brief The SlotDataBlock class
//! 64Words
class SlotDataBlock : public AbstractDataBlock
{
public:
    SlotDataBlock : public AbstractDataBlock() {}
};

namespace SlotBlock {
    Q_NAMESPACE

    enum DataBaseHeaders
    {
    ID=-1,
    //!

    };
Q_ENUM_NS(DataBaseHeaders)


}


#endif // DEFINITIONSLOTBLOCK_H

#ifndef DEFINITIONMANUALBLOCK_H
#define DEFINITIONMANUALBLOCK_H

#include <QObject>

#include <definitionbasicblocks.h>

//!
//! \brief The ManualModeDataBlock class
//! 256Words block
class ManualModeDataBlock :
        AbstractDataBlock
{
    Q_OBJECT
public:
    //!
    //! \brief The ManualContext enum
    //! Should do channel offset
    enum ManualContext
    {
        STATUS_WORD=0x00000000,
        CONTROL_WORD = 0x20,

        MON_CATEGRORY=0x28,
        MON_DEVICE_INDEX=0x29,

        COMMIT_MODE=0x2A,
        COMMIT_CATEGRORY=0x2C,
        COMMIT_DEVICE_INDEX=0x2E,

        DATA_BLOCK_HEAD=0x40,

        MONITOR_BLOCK_HEAD=0x80,
//        IO_MON_OVERRIDE=0x02000100,
    };
    Q_ENUM(ManualContext)
    enum StatusBits
    {
        BIT_0_ENGAGED_SEMI_AUTO = 0x000000 + STATUS_WORD,
        BIT_1_DONE = 0x010000 + STATUS_WORD,
        BIT_2_ENGAGED_MANUAL = 0x020000 + STATUS_WORD,
        BIT_3_INITIALIZED = 0x030000 + STATUS_WORD
    };
    enum ControlBits
    {
        BIT_0_ENGAGED_HMI = 0x000000 + CONTROL_WORD,
        BIT_1_RUN = 0x010000 + CONTROL_WORD
    };

    ManualModeDataBlock(QObject* parent = nullptr)
        : AbstractDataBlock(parent)
    {}
    ManualModeDataBlock(MODBUS_U_WORD* anchor,QObject* parent = nullptr)
        : AbstractDataBlock(anchor,parent)
    {}

    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case MON_CATEGRORY:
        case MON_DEVICE_INDEX:
        case COMMIT_MODE:
        case COMMIT_CATEGRORY:
        case COMMIT_DEVICE_INDEX:
            setData(key,value.value<MODBUS_U_WORD>());
            break;
        case DATA_BLOCK_HEAD:
            setData(key,value.value<CellDataBlock>());
        default:
            Bit(key,value.toBool());
            break;
        }
    }
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case MON_CATEGRORY:
        case MON_DEVICE_INDEX:
        case COMMIT_MODE:
        case COMMIT_CATEGRORY:
        case COMMIT_DEVICE_INDEX:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key));
            break;
        case MONITOR_BLOCK_HEAD:
            return QVariant::fromValue(getData<CellDataBlock>(key));
            break;
        default:
            return QVariant::fromValue(Bit(key));
            break;
        }
    }
};



#endif // DEFINITIONMANUALBLOCK_H

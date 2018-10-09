#ifndef DEFINITIONMANUALBLOCK_H
#define DEFINITIONMANUALBLOCK_H

#include <QObject>

#include <definitionbasicblocks.h>

//!
//! \brief The ManualModeDataBlock class
//! 256Words block
class ManualModeDataBlock :
        public AbstractDataBlock
{
    Q_OBJECT
public:
    enum Categrories : MODBUS_U_WORD
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_SIGNAL=2,

        SELECTION_COMMAND_BLOCK = 4,

        SELECTION_UNIT = 5,

        SELECTION_ALL = SELECTION_AXIS +
                        SELECTION_CYLINDER +
                        SELECTION_SIGNAL +
                        SELECTION_COMMAND_BLOCK +
                        SELECTION_UNIT,
    };
    Q_ENUM(Categrories)
    enum Mode : MODBUS_U_WORD
    {
        MODE_EXE_AXIS=SELECTION_AXIS,
        MODE_EXE_CYLINDER=SELECTION_CYLINDER,
        MODE_EXE_SIGNAL=SELECTION_SIGNAL, //dummy

        MODE_EXE_COMMAND_BLOCK=SELECTION_COMMAND_BLOCK,

        MODE_EXE_UNIT =SELECTION_UNIT,

        MODE_DOWNLOAD_DATA_BLOCK = 15, //PLC<-HMI
        MODE_UPLOAD_DATA_BLOCK = 16,   //PLC->HMI
    };
    Q_ENUM(Mode)
    //!
    //! \brief The ManualContext enum
    //! Should do channel offset
    enum ManualContext
    {
        STATUS_WORD=0x00000000,
        CONTROL_WORD = 32,

        MON_CATEGRORY=40,
        MON_DEVICE_INDEX=41,

        COMMIT_MODE=42,
        COMMIT_CATEGRORY=44,
        COMMIT_DEVICE_INDEX=46,

        DATA_BLOCK_HEAD=64,

        MONITOR_BLOCK_HEAD=128,
//        IO_MON_OVERRIDE=0x02000100,
    };
    Q_ENUM(ManualContext)
    enum StatusBits
    {
        BIT_0_ENGAGED_SEMI_AUTO = 0x000000 + STATUS_WORD,
        BIT_1_DONE = 0x010000 + STATUS_WORD,
        //! Depreciated
        BIT_2_ENGAGED_MANUAL = 0x020000 + STATUS_WORD,
        BIT_3_INITIALIZED = 0x030000 + STATUS_WORD
    };
    Q_ENUM(StatusBits)
    enum ControlBits
    {
        BIT_0_ENGAGED_HMI = 0x000000 + CONTROL_WORD,
        BIT_1_RUN = 0x010000 + CONTROL_WORD
    };
    Q_ENUM(ControlBits)
    enum TransferCommand
    {
        //! Clear all transfer task generated by DataChanged
        BATCH_PRESCHEDUALED_MODE = 0xffff01,
        //! Generate transfer task with some model , and clear them all
        BATCH_ALL_WRITE_MODE = 0xffff02,
        BATCH_ALL_READ_MODE = 0xffff04
    };
    Q_ENUM(TransferCommand)

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

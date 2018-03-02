#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QStateMachine>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

#include <modbuschannel.h>
#include <basicblocksdefinition.h>
#include <QVariant>



//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! Inherit QStateMachine
class ControllerManualMode : public QStateMachine
{
    Q_OBJECT
public:
    enum ManualContext : quint32
    {
        STATUS_WORD=0x00000000,
        ENGAGED_PLC=0x00000000,
        DONE=0x00010000,
        AXIS_ADR=0x00000001,
        POS_COMMAND=0x00000002,
        POS_FEEDBACK=0x00000004,
        SPD_FEEDBACK=0x00000006,
        TOR_FEEDBACK=0x00000008,
        ENGAGED_HMI=0x00000010,
        RUN=0x00010010,
        CANCEL=0x00020010,
        COMMIT_BLOCK=0x00000011,
        COMMIT_MODE=0x00000011,
        COMMIT_SELECTION=0x00000012,
        COMMIT_INDEX=0x00000013,
        DATA_BLOCK_HEAD=0x00000020,
        DATA_BLOCK_END=0x7F,
    };

    Q_ENUM(ManualContext)

    explicit ControllerManualMode(QObject *parent = nullptr);

    //!
    //! Data interfaces
    void CommitOption(CommitBlock value) {__commitOption = value;}
    CommitBlock CommitOption() const {return  __commitOption;}
    //!
    //! \brief blockCache
    //! Mind alignment problem

    void CommandBlock(ExtendedCommandBlock commandBlock)
    {
        //Value copy (memcopy?
        __commandBlock = commandBlock;
    }
    ExtendedCommandBlock CommandBlock() const
    {
        return __commandBlock;
    }
    AbstractMonitorBlock MonitorBlock() const
    {
        return __monitorBlock;
    }

signals:
    //!
    //! \brief readWordIn
    //! \param address
    //! \param wordData
    //!
    void requireReadData(AbstractAddress address,const QVariant data);
    //!
    //! \brief writeData
    //! \param address
    //! \param data
    //!
    void requireWriteData(AbstractAddress address,const QVariant data);
    //!
    //! \brief triggerOperation
    //! Linked to S1 transition condition
    void operationTriggered();
    void operationPerformed();
protected:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onMonitorBlockReply(UpdateEvent* event);

    ExtendedCommandBlock __commandBlock;
    AbstractMonitorBlock __monitorBlock;

    CommitBlock __commitOption;

    ModbusChannel* channel; //TODO
};

#endif // CONTROLLERMANUALMODE_H

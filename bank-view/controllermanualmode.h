#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QMap>
#include <QStateMachine>
#include <baselayerdefinitions.h>
using namespace BaseLayer;

#include <modbuschannel.h>
#include <basicblocksdefinition.h>
#include <QVariant>
#include <QTimer>


//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! Inherit QStateMachine
class ControllerManualMode : public QStateMachine
{
    Q_OBJECT
public:

    //!
    //! \brief The ManualContext enum
    //! Should do channel offset
    enum ManualContext : quint32
    {
        STATUS_WORD=0x02000000,
        ENGAGED_PLC=0x02000000,
        DONE=0x02010000,
        AXIS_ADR=0x02000001,
        POS_COMMAND=0x02000002,
        POS_FEEDBACK=0x02000004,
        SPD_FEEDBACK=0x02000006,
        TOR_FEEDBACK=0x02000008,
        ENGAGED_HMI=0x02000010,
        RUN=0x02010010,
        CANCEL=0x02020010,
        COMMIT_BLOCK=0x02000011,
        COMMIT_MODE=0x02000011,
        COMMIT_SELECTION=0x02000012,
        COMMIT_INDEX=0x02000013,
        DATA_BLOCK_HEAD=0x02000020,
    };
    Q_ENUM(ManualContext)
    enum ManualState : int
    {
        STATE_IN_AUTO,
        STATE_IDLE,
        STATE_COMPLETE,
        STATE_FINISH,
    };
    Q_ENUM(ManualState)


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

    ManualState CurrentState() const { return __currentState;}


    static ControllerManualMode* Instance();
public slots:
    void onInterrupted() { emit requireWriteData(ModbusDriverAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(false));}
    void onMonitorAxisChanged(MODBUS_WORD objectId){emit requireWriteData(ModbusDriverAddress(ControllerManualMode::AXIS_ADR),QVariant::fromValue(objectId));}
signals:
    //!
    //! \brief readWordIn
    //! \param address
    //! \param wordData
    //!
    void requireReadData(ModbusDriverAddress address,const QVariant data);
    //!
    //! \brief writeData
    //! \param address
    //! \param data
    //!
    void requireWriteData(ModbusDriverAddress address,const QVariant data);
    //!
    //! \brief triggerOperation
    //! Linked to S1 transition condition
    void operationTriggered();
    void operationPerformed();
protected slots:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onReply(UpdateEvent* event);
protected:
     explicit ControllerManualMode(QObject *parent = nullptr);

    ExtendedCommandBlock __commandBlock;
    AbstractMonitorBlock __monitorBlock;

    CommitBlock __commitOption;

    QMap<ManualState,QState*> __stateMap;
    ManualState __currentState;

    ModbusChannel* __channel;

    static ControllerManualMode* __instance;
};

#endif // CONTROLLERMANUALMODE_H

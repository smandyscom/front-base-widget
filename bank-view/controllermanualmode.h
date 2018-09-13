#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QMap>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>

#include <definitionsbaselayer.h>
using namespace BaseLayer;

#include <controllerbase.h>


#include <utilities.h>

//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! Inherit QStateMachine
class ControllerManualMode :
        public ControllerBase
{
    Q_OBJECT
public:
//    //!
//    //! \brief The ManualContext enum
//    //! Should do channel offset
//    enum ManualContext : quint32
//    {
//        STATUS_WORD=0x02000000,
//        ENGAGED_SEMI_AUTO=0x02000000,
//        DONE=0x02010000,
//        ENGAGED_MANUAL = 0x02020000,
//        INITIALIZED = 0x02030000,
//        ENGAGED_HMI=0x02000020,
//        RUN=0x02010020,
//        MON_CATEGRORY=0x02000028,
//        MON_DEVICE_INDEX=0x02000029,
//        COMMIT_BLOCK=0x200002A,
//        COMMIT_MODE=0x200002A,
//        COMMIT_SELECTION=0x200002C,
//        COMMIT_INDEX=0x200002E,
//        DATA_BLOCK_HEAD=0x02000040,
//        MONITOR_BLOCK_HEAD=0x02000080,
//        IO_MON_OVERRIDE=0x02000100,
//    };
//    Q_ENUM(ManualContext)
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
    void CommitOption(CommitBlock value) {__channel->Access<CommitBlock>(ModbusDriverAddress(COMMIT_BLOCK),value);}
    CommitBlock CommitOption() const {return  __channel->Access<CommitBlock>(ModbusDriverAddress(COMMIT_BLOCK));}

    //!
    //! \brief DataBlock
    //! \param value
    //! Unpack and write in
    void DataBlock(QVariant value){__channel->Access(ModbusDriverAddress(DATA_BLOCK_HEAD),value);}
    //!
    //!
    //!
    template<typename T>
    QVariant DataBlock() const
    {
        return QVariant::fromValue(__channel->Access<T>(ModbusDriverAddress(DATA_BLOCK_HEAD)));
    }
    CellDataBlock MonitorBlock() const
    {
        return __channel->Access<CellDataBlock>(ModbusDriverAddress(MONITOR_BLOCK_HEAD));
    }
    void MonitorDeviceCategrory(MODBUS_U_WORD value)
    {
        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::MON_CATEGRORY),QVariant::fromValue(value));
    }

    ManualState CurrentState() const { return __currentState;}

//    //!
//    //! \brief IsChannelActivated
//    //! \return
//    //!
//    bool IsSemiAutoActivated() const
//    {
//        return __currentState == STATE_IDLE;
//    }
//    //!
//    //! \brief IsManualModeActiavted
//    //! \return
//    //!
//    bool IsManualModeActiavted() const
//    {
//        return IsSemiAutoActivated() && __channel->Access<bool>(ModbusDriverAddress(ENGAGED_MANUAL));
//    }
//    bool IsControllerInitialized() const
//    {
//        return __channel->Access<bool>(ModbusDriverAddress(INITIALIZED));
//    }

//    static ControllerManualMode* Instance();
public slots:
    void onInterrupted() {
        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(false));
    }
    void onMonitorDeviceIndexChanged(MODBUS_U_WORD objectId){
        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::MON_DEVICE_INDEX),QVariant::fromValue(objectId));
    }
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
//protected slots:
//    //!
//    //! \brief onMonitorBlockReply
//    //! \param event
//    //! Looping
//    void onReply();
protected:
     explicit ControllerManualMode(QObject *parent = nullptr);

    QMap<ManualState,QState*> __stateMap;
    ManualState __currentState;

    QStateMachine* m_stateMachine;
//    ModbusChannel* __channel;

//    static ControllerManualMode* __instance;
};

#endif // CONTROLLERMANUALMODE_H

#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QMap>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>

#include <definitionmanualblock.h>

using namespace BaseLayer;

#include <controllerbase.h>


#include <utilities.h>

//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! How pass dynamic property to multiple parent
//!
class ControllerManualMode :
        public ControllerBase
{
    Q_OBJECT
public:
    enum ManualState : int
    {
        STATE_IN_AUTO,
        STATE_IDLE,
        STATE_COMPLETE,
        STATE_FINISH,
    };
    Q_ENUM(ManualState)



    explicit ControllerManualMode(QObject *parent = nullptr);
    //!
    //! Data interfaces
//    void CommitOption(CommitBlock value) {__channel->Access<CommitBlock>(ModbusDriverAddress(COMMIT_BLOCK),value);}
//    CommitBlock CommitOption() const {return  __channel->Access<CommitBlock>(ModbusDriverAddress(COMMIT_BLOCK));}

    //!
    //! \brief DataBlock
    //! \param value
    //! Unpack and write in
//    void DataBlock(QVariant value){__channel->Access(ModbusDriverAddress(DATA_BLOCK_HEAD),value);}
    //!
    //!
    //!
//    template<typename T>
//    QVariant DataBlock() const
//    {
//        return QVariant::fromValue(__channel->Access<T>(ModbusDriverAddress(DATA_BLOCK_HEAD)));
//    }
//    CellDataBlock MonitorBlock() const
//    {
//        return __channel->Access<CellDataBlock>(ModbusDriverAddress(MONITOR_BLOCK_HEAD));
//    }
//    void MonitorDeviceCategrory(MODBUS_U_WORD value)
//    {
//        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::MON_CATEGRORY),QVariant::fromValue(value));
//    }

    ManualState CurrentState() const { return m_currentState;}

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
//public slots:
//    void onInterrupted() {
//        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(false));
//    }
//    void onMonitorDeviceIndexChanged(MODBUS_U_WORD objectId){
//        emit requireWriteData(ModbusDriverAddress(ControllerManualMode::MON_DEVICE_INDEX),QVariant::fromValue(objectId));
//    }
signals:
//    //!
//    //! \brief readWordIn
//    //! \param address
//    //! \param wordData
//    //!
//    void requireReadData(ModbusDriverAddress address,const QVariant data);
//    //!
//    //! \brief writeData
//    //! \param address
//    //! \param data
//    //!
//    void requireWriteData(ModbusDriverAddress address,const QVariant data);
    //!
    //! \brief triggerOperation
    //! Linked to S1 transition condition
    void operationTriggered();
//    void operationPerformed();
    void operationReady();
//protected slots:
//    //!
//    //! \brief onMonitorBlockReply
//    //! \param event
//    //! Looping
//    void onReply();

protected:

    QMap<ManualState,QState*> m_stateMap;
    ManualState m_currentState;

    QStateMachine* m_stateMachine;
    //!
    //! \brief s1Entered
    //! PLC ready
    virtual void plcReady();
    //!
    //! \brief s2Existed
    //! Run on
    virtual void runOn();
    //!
    //! \brief s2Exited
    //! Done on
    virtual void doneOn();
    //!
    //! \brief s3Exited
    //! Done off
    virtual void doneOff();

    //!
    //! \brief m_monitor_categrories
    //! Polymorphism of monitor data block
//    QMap<ManualModeDataBlock::Categrories,AbstractDataBlock*> m_monitor_categrories;
    //!
    //! \brief m_monitor_propertyValues
    //! \param key
    //! \return
    //! Overrides
//    QVariant m_monitor_propertyValues(QVariant key) Q_DECL_OVERRIDE;
    void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERMANUALMODE_H

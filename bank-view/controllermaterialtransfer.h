#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>
#include <QStateMachine>

#include <QSqlRelationalTableModel>

#include <modbuschannel.h>
#include <definitionslotblock.h>
#include <abstractsqltableadpater.h>
//!
//! \brief The ControllerMaterialTransfer class
//! Proceding material/slot handshaking
class ControllerMaterialTransfer :
        public QStateMachine
{
    Q_OBJECT
public:
    enum Role
    {
        CREATE,
        DATA,
    };
    enum SlotContext
    {
        //! PLC->DB
        PLC_ENGAGED,
        ACT,
        IS_VALID,
        WORD_OUT,
        SYNC_ACTION,
        //! DB->PLC
        DB_ENGAGED,
        WORD_IN,
        DONE,
        //! Mutual
        MATERIAL_ID,
        DATA,
    };
    enum SyncRequests
    {
        ACTION_UPDATE_HEADER,
        ACTION_UPDATE_BLOCK,
        ACTION_CREATE,
        ACTION_QUERY,
    };
    enum SlotState
    {
        WAIT_ACT_ON,
        SYNC,
        WAIT_ACT_OFF,
    };

    explicit ControllerMaterialTransfer(QObject *parent = nullptr);

    //! TODO , destrcutor , DB disengaged

    //! Current material id this slot held
    int SlotMaterialId() const
    {
        return __slotMaterialId;
    }
protected slots:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onReply(UpdateEvent* event);
protected:
    ModbusChannel* __channel;

    int __slotOffset;
    int __slotMaterialId;
    bool __slotIsValid;
    SyncRequests __slotRequest;

    QSqlRelationalTableModel* __table;

    ModbusDriverAddress toOffseteAaddress(int base);

    AbstractSqlTableAdpater* __adpator;

    QMap<SlotState,QState*> __stateMap;
    SlotState __currentState;
};

#endif // CONTROLLERMATERIALTRANSFER_H

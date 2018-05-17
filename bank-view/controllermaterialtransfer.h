#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>
#include <QStateMachine>

#include <QSqlRelationalTableModel>
#include <QFileInfo>

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
    enum SlotType
    {
        //! Update header only , no backend data table
        TYPE_EMPTY_NODE,
        //! With backend data table
        TYPE_DATA_NODE,
    };
    enum NameConstants
    {
        //! Data base name
        //!
        MAT_DATA_SLOT,
        MAT_HEADER_SLOT
    };
    enum SlotContext
    {
        //! PLC->DB
        PLC_ENGAGED=0x3008000,
        ACT=0x3018000,
        IS_VALID=0x3028000,
        WORD_OUT=0x3008000,
        SYNC_ACTION=0x3008001,
        //! PLC<-DB
        DB_ENGAGED=0x3008008,
        WORD_IN=0x3008008,
        DONE=0x3018008,
        //! Mutual
        MATERIAL_ID=0x3008010,
        BLOCK_DATA=0x3008014,
    };
    enum SyncRequests
    {
        ACTION_UPDATE_HEADER = 0x01,
        ACTION_UPDATE_BLOCK = 0x02,
        ACTION_CREATE = 0x04,
        ACTION_QUERY = 0x08,
    };
    enum SlotState
    {
        WAIT_ACT_ON,
        SYNC,
        WAIT_ACT_OFF,
    };
    Q_ENUM(SlotContext)
    Q_ENUM(SyncRequests)
    Q_ENUM(SlotState)
    Q_ENUM(NameConstants)

    explicit ControllerMaterialTransfer(int slotOffset,
                                        SlotType role,
                                        QObject *parent = nullptr);
    ~ControllerMaterialTransfer();

    //! TODO , destrcutor , DB disengaged

    //! Current material id this slot held
    int MaterialId() const
    {
        return __materialId;
    }
    bool IsValid() const
    {
        return __isValid;
    }

protected slots:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onReply();
    void onPrimeInsert(int row, QSqlRecord &record);
protected:
    ModbusChannel* __channel;

    int __index;
    int __materialId;
    bool __isValid;
    SlotType __role;

    SyncRequests __request;

    QSqlRelationalTableModel* __table;

    ModbusDriverAddress toOffseteAaddress(int base);

    AbstractSqlTableAdpater* __adpator;

    QMap<SlotState,QState*> __stateMap;
    SlotState __currentState;

    static QSqlDatabase __database;
    static QString __databaseName;
};

#endif // CONTROLLERMATERIALTRANSFER_H

#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>
#include <QStateMachine>

#include <QSqlRelationalTableModel>
#include <QFileInfo>
#include <QtConcurrent>
#include <QElapsedTimer>

#include <modbuschannel.h>
#include <definitionslotblock.h>
#include <abstractsqltableadpater.h>

class MaterialHeaderBlock
{
public:
    MaterialHeaderBlock() {}
protected:
    MODBUS_U_WORD reserved[96];
};
Q_DECLARE_METATYPE(MaterialHeaderBlock)

//!
//! \brief The ControllerMaterialTransfer class
//! Proceding material/slot handshaking
class ControllerMaterialTransfer :
        public QStateMachine
{
    Q_OBJECT
public:
//    enum SlotType
//    {
//        //! Update header only , no backend data table
//        TYPE_EMPTY_NODE=0x02,
//        //! With backend data table
//        TYPE_DATA_NODE=0x01,
//        //!
//        ATTR_CREATE_NODE=0x10,
//        TYPE_CREATE_NODE = ATTR_CREATE_NODE+TYPE_DATA_NODE,
//    };
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
        //PLC_ENGAGED=0x4008000,
        ACT=0x4018000,
        IS_VALID=0x4028000,
        WORD_OUT=0x4008000,
        SYNC_ACTION=0x4008001,
        //! PLC<-DB
        DB_ENGAGED=0x4008008,
        WORD_IN=0x4008008,
        DONE=0x4018008,
        //! Mutual
        MATERIAL_ID=0x4008010, //move to first 8 words
        BLOCK_DATA=0x4008014,
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
        WAIT_ACT_OFF,
    };
    Q_ENUM(SlotContext)
    Q_ENUM(SyncRequests)
    Q_ENUM(SlotState)
    Q_ENUM(NameConstants)

    explicit ControllerMaterialTransfer(int slotOffset,
                                        int channelIndex,
                                        QObject *parent = nullptr);
    ~ControllerMaterialTransfer();

    //! Current material id this slot held
    int MaterialId() const
    {
        return __materialId;
    }
    int Index() const
    {
        return __slotIndex;
    }
    bool IsValid() const
    {
        return __isValid;
    }
    bool ConnectionEngaged() const
    {
        return __connectionEngaged;
    }
//    SlotType Role() const
//    {
//        return __role;
//    }
    QSqlTableModel* Table() const
    {
        return __table;
    }

    static QSqlDatabase DataBase()
    {
        return __database;
    }
signals:
    void dataUpdated();
public slots:
    void onAboutToLeave();
    void onInsert();
    void onQuery();
    void onUpdate();
protected slots:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onReply();

protected:
    ModbusChannel* __channel;

    int __slotIndex;
    int __channelIndex;

    int __materialId;
    bool __isValid;
    //SlotType __role;

    SyncRequests __request;

    QSqlTableModel* __table;

    ModbusDriverAddress toOffseteAddress(int base);

    AbstractSqlTableAdpater* __adpator;

    QMap<SlotState,QState*> __stateMap;
    SlotState __currentState;

    static QSqlDatabase __database;
    static QString __databaseName;

    bool __connectionEngaged;

    int __pollCyclic;

    AbstractDataBlock __adb;

    QElapsedTimer __procedureTimer;
};


#endif // CONTROLLERMATERIALTRANSFER_H

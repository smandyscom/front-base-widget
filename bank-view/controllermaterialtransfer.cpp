#include "controllermaterialtransfer.h"
#include <QDebug>
ControllerMaterialTransfer::ControllerMaterialTransfer(int index,SlotType role, QObject *parent) :
    QStateMachine(parent),
    __index(index),
    __role(role),
    __pollCyclic(50)
{
    //! Channel initialize
    __channel = ModbusChannel::Instance();

    //! Open table
    if(!__database.isOpen())
    {
        __database = QSqlDatabase::addDatabase("QSQLITE","material");
        __database.setDatabaseName(__databaseName);
        QFileInfo qf(__databaseName);
        if(!qf.exists())
            qDebug() << QString("%1 not existed").arg(qf.absoluteFilePath());
        bool result = __database.open();
        qDebug() << result;
    }
    if(((__role & TYPE_DATA_NODE) >0) && __database.isOpen())
    {
        __table = new QSqlRelationalTableModel(this,__database);
        QString __tableName = QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index);
        __table->setTable(QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index));
        bool result = __table->select();
        __adpator = new GenericSqlTableAdapter<SlotDataBlock,SlotBlock::DataBaseHeaders>(__table);
        if(!result)
            qDebug() << result;
    }
    //! Very first shot
    __channel->beginAccess<MODBUS_U_WORD>(toOffseteAddress(WORD_OUT));
    //! DB engaged,
    if((__role & TYPE_DATA_NODE)>0)
        __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),true);
    __connectionEngaged = true;
    //!
    QState* s0 = new QState(this);
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    __stateMap[WAIT_ACT_ON] = s0;
    __stateMap[SYNC] = s1;
    __stateMap[WAIT_ACT_OFF] = s2;
    //! Common
    foreach (QState* var, __stateMap.values()) {
        connect(var,&QState::entered,[this](){
           __currentState = __stateMap.key(qobject_cast<QState*>(sender()));
        });
    }
    //!
    //! s0
    ValueTransition* actOn = new ValueTransition(toOffseteAddress(ACT),ValueTransition::BIT_STATE_ON);
    actOn->setTargetState(s1);
    s0->addTransition(actOn);
    if((__role & ATTR_CREATE_NODE) > 0)
        connect(s0,&QState::entered,[this](){
        //!Fetch next id
        onPrimeInsert();
        });
    connect(s0,&QState::exited,[this](){
       //! read SYNC_ACTION , ID , IS_VALID
//       __channel->beginAccess<MODBUS_U_LONG>(toOffseteAddress(MATERIAL_ID));
//       __channel->beginAccess<MODBUS_U_WORD>(toOffseteAddress(IS_VALID));
//       __channel->beginAccess<MODBUS_U_WORD>(toOffseteAddress(SYNC_ACTION));
        __channel->beginAccess<CellDataBlock>(toOffseteAddress(PLC_ENGAGED));
    });
    //!
    //!  s1
    ValueTransition* syncRequest = new ValueTransition(toOffseteAddress(PLC_ENGAGED),ValueTransition::VALUE_UPDATED);
    syncRequest->setTargetState(s2);
    s1->addTransition(syncRequest);

    connect(s1,&QState::exited,[this]()
    {
        __materialId = __channel->Access<MODBUS_U_WORD>(toOffseteAddress(MATERIAL_ID));
        __isValid = __channel->Access<bool>(toOffseteAddress(IS_VALID));
        __request = SyncRequests(__channel->Access<MODBUS_U_WORD>(toOffseteAddress(SYNC_ACTION)));

        //! According sync request perform DB manipulation
        switch (__request) {
        case ACTION_UPDATE_HEADER:
            //! slot held ID/IS_VALID updated only
            break;
        case ACTION_UPDATE_BLOCK:
            //! write into data-base
            __channel->beginAccess<CellDataBlock>(toOffseteAddress(BLOCK_DATA));
            break;
        case ACTION_CREATE:
            __channel->Access<MODBUS_U_LONG>(toOffseteAddress(MATERIAL_ID),__materialId);
            break;
        case ACTION_QUERY:
            //!Write data
            //! find record by material id
            __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA),
                                             static_cast<CellDataBlock>(
                                                 __adpator->Record(__materialId,
                                                                   AbstractSqlTableAdpater::KEY_NAMED_KEY,
                                                                   QVariant::fromValue(SlotBlock::ID))
                                                 )
                                             );
            break;
        default:
            break;
        }

        //! Acknoledge
        __channel->Access<bool>(toOffseteAddress(DONE),true);
        __pollCyclic=1;//speed up
    });

    //!
    //! s2
    ValueTransition* actOff = new ValueTransition(toOffseteAddress(ACT),ValueTransition::BIT_STATE_OFF);
    actOff->setTargetState(s0);
    s2->addTransition(actOff);
    connect(s2,&QState::exited,[this](){

        switch (__request) {
        case ACTION_UPDATE_BLOCK:
        {
//            AbstractDataBlock __adb;
//            *static_cast<CellDataBlock*>(&__adb) =
//                    __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA));
//            //! Write in data base
//            __adpator->Record(__materialId,
//                              __adb,
//                              AbstractSqlTableAdpater::KEY_NAMED_KEY,
//                              QVariant::fromValue(SlotBlock::ID));
            break;
        }
        default:
            break;
        }

        //! Rewind
       __channel->Access<bool>(toOffseteAddress(DONE),false);
       __pollCyclic=50;//slow down
       emit dataUpdated();
    });

    //! On reply
    connect(__channel,&ModbusChannel::readReply,this,&ControllerMaterialTransfer::onReply);
    //!Ready
    setInitialState(s0);
    if((__role & TYPE_DATA_NODE) >0)
        start();

    //!prefetch id
    if((__role & ATTR_CREATE_NODE) > 0)
        onPrimeInsert();
}

ControllerMaterialTransfer::~ControllerMaterialTransfer()
{

}

ModbusDriverAddress ControllerMaterialTransfer::toOffseteAddress(int base)
{
    return ModbusDriverAddress(base + __index * 256);
}

void ControllerMaterialTransfer::onReply()
{
    if(__channel->CachedReplyAddress().getAddress() ==
            toOffseteAddress(DB_ENGAGED))
    {
        if(__channel->Access<bool>(toOffseteAddress(DB_ENGAGED)))
        {
            onAboutToLeave();//trigger again
        }
        else
        {
            __connectionEngaged = false;
        }
    }

    if(__channel->CachedReplyAddress().getAddress() !=
            toOffseteAddress(WORD_OUT))
        return;
    QTimer::singleShot(__pollCyclic,[this](){
        __channel->beginAccess<MODBUS_U_WORD>(toOffseteAddress(WORD_OUT));
    });
}

void ControllerMaterialTransfer::onPrimeInsert()
{
    //!insert record on table
    bool result = __table->insertRecord(-1,__table->record());
    __table->sort(0,Qt::AscendingOrder);
    QSqlRecord __record = __table->record(__table->rowCount()-1);
    __materialId = __record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();
    //__channel->Access<MODBUS_U_LONG>(toOffseteAddress(MATERIAL_ID),__materialId);
}

void ControllerMaterialTransfer::onAboutToLeave()
{
    __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),false);
    __channel->beginAccess<bool>(toOffseteAddress(DB_ENGAGED)); //double-check
}

QSqlDatabase ControllerMaterialTransfer::__database;
QString ControllerMaterialTransfer::__databaseName = "material.db";

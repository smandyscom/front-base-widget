#include "controllermaterialtransfer.h"
#include <QDebug>
ControllerMaterialTransfer::ControllerMaterialTransfer(int index,SlotType role, QObject *parent) :
    QStateMachine(parent),
    __index(index),
    __role(role)
{
    //! Channel initialize
    __channel = ModbusChannel::Instance();

    //! Open table
    if(!__database.isOpen())
    {
        __database = QSqlDatabase::database();
        __database.setDatabaseName(__databaseName);
        QFileInfo qf(__databaseName);
        if(!qf.exists())
            qDebug() << QString("%1 not existed").arg(qf.absoluteFilePath());
        bool result = __database.open();
        qDebug() << result;
    }
    if(__role == TYPE_DATA_NODE)
    {
        __table = new QSqlRelationalTableModel(this,__database);
        QString __tableName = QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index);
        __table->setTable(QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index));
        bool result = __table->select();
        __adpator = new GenericSqlTableAdapter<SlotDataBlock,SlotBlock::DataBaseHeaders>(__table);
        //connect(__table,&QSqlRelationalTableModel::primeInsert,this,&ControllerMaterialTransfer::onPrimeInsert);
    }
    //! Very first shot
    __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(WORD_OUT));
    //! DB engaged,
    __channel->Access<bool>(toOffseteAaddress(DB_ENGAGED),true);
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
    ValueTransition* actOn = new ValueTransition(toOffseteAaddress(ACT),ValueTransition::BIT_STATE_ON);
    actOn->setTargetState(s1);
    s0->addTransition(actOn);
    connect(s0,&QState::exited,[this](){
       //! read SYNC_ACTION , ID , IS_VALID
       __channel->beginAccess<MODBUS_U_LONG>(toOffseteAaddress(MATERIAL_ID));
       __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(IS_VALID));
       __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(SYNC_ACTION));
    });
    //!
    //!  s1
    ValueTransition* syncRequest = new ValueTransition(toOffseteAaddress(SYNC_ACTION),ValueTransition::VALUE_UPDATED);
    syncRequest->setTargetState(s2);
    s1->addTransition(syncRequest);
    connect(s1,&QState::exited,[this]()
    {
        __materialId = __channel->Access<MODBUS_U_WORD>(toOffseteAaddress(MATERIAL_ID));
        __isValid = __channel->Access<bool>(toOffseteAaddress(IS_VALID));
        __request = SyncRequests(__channel->Access<MODBUS_U_WORD>(toOffseteAaddress(SYNC_ACTION)));

        //! According sync request perform DB manipulation
        switch (__request) {
        case ACTION_UPDATE_HEADER:
            //! slot held ID/IS_VALID updated only
            break;
        case ACTION_UPDATE_BLOCK:
            //! write into data-base
            __channel->beginAccess<CellDataBlock>(toOffseteAaddress(BLOCK_DATA));
            break;
        case ACTION_CREATE:
        {
            //!insert record on table
           bool result = __table->insertRecord(-1,__table->record());
           __table->sort(0,Qt::AscendingOrder);
//            QSqlRecord __record = __table->record(__table->rowCount()-1);

//            __materialId = __record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();
//            qDebug() << __materialId;
//            __channel->Access<MODBUS_U_LONG>(toOffseteAaddress(MATERIAL_ID),
//                                             __materialId);
            QSqlRecord __record = __table->record(__table->rowCount()-1);
            onPrimeInsert(0,__record);
            break;
        }

        case ACTION_QUERY:
            //!Write data
            //! find record by material id
            __channel->Access<CellDataBlock>(toOffseteAaddress(BLOCK_DATA),
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
        __channel->Access<bool>(toOffseteAaddress(DONE),true);
    });

    //!
    //! s2
    ValueTransition* actOff = new ValueTransition(toOffseteAaddress(ACT),ValueTransition::BIT_STATE_OFF);
    actOff->setTargetState(s0);
    s2->addTransition(actOff);
    connect(s2,&QState::exited,[this](){

        switch (__request) {
        case ACTION_UPDATE_BLOCK:
        {
            AbstractDataBlock __adb;
            *static_cast<CellDataBlock*>(&__adb) =
                    __channel->Access<CellDataBlock>(toOffseteAaddress(BLOCK_DATA));
            //! Write in data base
            __adpator->Record(__materialId,
                              __adb,
                              AbstractSqlTableAdpater::KEY_NAMED_KEY,
                              QVariant::fromValue(SlotBlock::ID));
            break;
        }
        default:
            break;
        }

        //! Rewind
       __channel->Access<bool>(toOffseteAaddress(DONE),false);
    });

    //! On reply
    connect(__channel,&ModbusChannel::readReply,this,&ControllerMaterialTransfer::onReply);
    //!Ready
    setInitialState(s0);
    start();
}

ControllerMaterialTransfer::~ControllerMaterialTransfer()
{
    __channel->Access<bool>(toOffseteAaddress(DB_ENGAGED),false);
}

ModbusDriverAddress ControllerMaterialTransfer::toOffseteAaddress(int base)
{
    return ModbusDriverAddress(base + __index * 256);
}

void ControllerMaterialTransfer::onReply()
{
    if(__channel->CachedReplyAddress().getAddress() !=
            toOffseteAaddress(WORD_OUT))
        return;
    QTimer::singleShot(10,[this](){
        __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(WORD_OUT));
    });
}

void ControllerMaterialTransfer::onPrimeInsert(int row, QSqlRecord &record)
{
    __materialId = record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();
    __channel->Access<MODBUS_U_LONG>(toOffseteAaddress(MATERIAL_ID),__materialId);
}

QSqlDatabase ControllerMaterialTransfer::__database;
QString ControllerMaterialTransfer::__databaseName = "material.db";

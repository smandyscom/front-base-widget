#include "controllermaterialtransfer.h"
#include <QDebug>
ControllerMaterialTransfer::ControllerMaterialTransfer(int index,SlotType role, QObject *parent) :
    QStateMachine(parent),
    __index(index),
    __role(role),
    __pollCyclic(10),
    __materialId(0)
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
        __table = new QSqlTableModel(this,__database);
        QString __tableName = QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index);
        __table->setTable(QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__index));
        bool result = __table->select();
        __adpator = new GenericSqlTableAdapter<SlotDataBlock,SlotBlock::DataBaseHeaders>(__table);
        if(!result)
            qDebug() << result;
    }

    if((__role & TYPE_DATA_NODE)>0)
    {
        //! DB engaged,
        __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),true);
        //! Very first shot
        __channel->beginAccess<MaterialHeaderBlock>(toOffseteAddress(WORD_OUT));
        __connectionEngaged = true;
    }

    //!
    QState* s0 = new QState(this);
    QState* s1 = new QState(this);
    __stateMap[WAIT_ACT_ON] = s0;
    __stateMap[WAIT_ACT_OFF] = s1;
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

    connect(s0,&QState::exited,[this]()
    {
        __procedureTimer.start();

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
            onInsert();
            __channel->Access<MODBUS_U_LONG>(toOffseteAddress(MATERIAL_ID),__materialId);
            break;
        case ACTION_QUERY:
            //!Write data
            //! find record by material id
            onQuery();
            __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA),static_cast<CellDataBlock>(__adb));
            break;
        default:
            break;
        }

        //! Acknoledge
        __channel->Access<bool>(toOffseteAddress(DONE),true);
        __pollCyclic=1;//speed up
    });

    //!
    //! s1
    ValueTransition* actOff = new ValueTransition(toOffseteAddress(ACT),ValueTransition::BIT_STATE_OFF);
    actOff->setTargetState(s0);
    s1->addTransition(actOff);
    connect(s1,&QState::exited,[this](){

        switch (__request) {
        case ACTION_UPDATE_BLOCK:
        {
            *static_cast<CellDataBlock*>(&__adb) =
                    __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA));
            //! Rewind
            __channel->Access<bool>(toOffseteAddress(DONE),false); //lead
            onUpdate();
            break;
        }
        default:
            __channel->Access<bool>(toOffseteAddress(DONE),false); //lead
            break;
        }

       __pollCyclic=10;//slow down
       emit dataUpdated();

       qDebug() << QString("%1,procedure elapsed,%2").arg(__index).arg(__procedureTimer.elapsed());
    });

    //! On reply
    connect(__channel,&ModbusChannel::readReply,this,&ControllerMaterialTransfer::onReply);
    //!Ready
    setInitialState(s0);
    if((__role & TYPE_DATA_NODE)>0)
        start();

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
        __channel->beginAccess<MaterialHeaderBlock>(toOffseteAddress(WORD_OUT));
    });
}

void ControllerMaterialTransfer::onInsert()
{
    QElapsedTimer __timer;
    __timer.start();

    __table->database().transaction();

    //!insert record on table
    QSqlRecord __record = __table->record();
    __record.setGenerated(QVariant::fromValue(SlotBlock::ID).toString(),false);
    if(!__table->insertRecord(-1,__record))
        qDebug() << QString("Insert record error , %1").arg(__index);
    __table->select();//re-select

    __record = __table->record(__table->rowCount()-1);
    __materialId = __record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();

    if(!__table->database().commit())
        qDebug() << QString("database commit failed onInsert");
    qDebug() << QString("%1,onInsert elapsed,%2").arg(__index).arg(__timer.elapsed());


}
void ControllerMaterialTransfer::onQuery()
{
    QElapsedTimer __timer;
    __timer.start();

    __table->database().transaction();
    __adb = __adpator->Record(__materialId,
                              AbstractSqlTableAdpater::KEY_NAMED_KEY,
                              QVariant::fromValue(SlotBlock::ID));

    if(!__table->database().commit())
        qDebug() << QString("database commit failed onInsert");
    qDebug() << QString("%1,onQuery elapsed,%2").arg(__index).arg(__timer.elapsed());
}
void ControllerMaterialTransfer::onUpdate()
{
    QElapsedTimer __timer;
    __timer.start();

    __table->database().transaction();

    //! Write in data base
    __adpator->Record(__materialId,
                      __adb,
                      AbstractSqlTableAdpater::KEY_NAMED_KEY,
                      QVariant::fromValue(SlotBlock::ID));

    if(!__table->database().commit())
        qDebug() << QString("database commit failed onInsert");

    qDebug() << QString("%1,onUpdate elapsed,%2").arg(__index).arg(__timer.elapsed());
}

void ControllerMaterialTransfer::onAboutToLeave()
{
    __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),false);
    __channel->beginAccess<bool>(toOffseteAddress(DB_ENGAGED)); //double-check
}

QSqlDatabase ControllerMaterialTransfer::__database;
QString ControllerMaterialTransfer::__databaseName = "material.db";

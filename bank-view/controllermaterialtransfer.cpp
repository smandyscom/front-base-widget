#include "controllermaterialtransfer.h"
#include <QDebug>
ControllerMaterialTransfer::ControllerMaterialTransfer(int index, int channelIndex, QObject *parent) :
    QStateMachine(parent),
    __slotIndex(index),
    __pollCyclic(100),
    __materialId(0),
    __channelIndex(channelIndex),
    m_totalCounter(0),
    m_okCounter(0),
    m_ngCounter(0),
    m_currentGrade(NG),
    m_index_grade1(0),
    m_index_grade2(0)
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
    if(__database.isOpen())
    {
        __table = new QSqlTableModel(this,__database);
        QString __tableName = QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__slotIndex);
        __table->setTable(QString("%1%2").arg(QVariant::fromValue(MAT_DATA_SLOT).toString()).arg(__slotIndex));
        //bool result = __table->select();
        __adpator = new GenericSqlTableAdapter<SlotDataBlock,SlotBlock::DataBaseHeaders>(__table);
//        if(!result)
//            qDebug() << result;
    }

//    if((__role & TYPE_DATA_NODE)>0)
//    {


        //! Very first shot
        __channel->beginAccess<MaterialHeaderBlock>(toOffseteAddress(WORD_OUT));
    //}

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

    connect(s0,&QState::entered,[=]()
    {
        //! set DB engaged,
        __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),true);
    });
    connect(s0,&QState::exited,[this]()
    {
        __procedureTimer.start();

        __materialId = __channel->Access<MODBUS_U_WORD>(toOffseteAddress(MATERIAL_ID));
        __isValid = __channel->Access<bool>(toOffseteAddress(IS_VALID));
        __request = SyncRequests(__channel->Access<MODBUS_U_WORD>(toOffseteAddress(SYNC_ACTION)));

        //! Count up
        m_totalCounter +=1;

        //! According sync request perform DB manipulation
        switch (__request) {
        case ACTION_UPDATE_HEADER:
            //! slot held ID/IS_VALID updated only
            break;
        case ACTION_UPDATE_BLOCK:
            //! write into data-base
            //__channel->beginAccess<CellDataBlock>(toOffseteAddress(BLOCK_DATA));
            *static_cast<CellDataBlock*>(&__adb) =
                    __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA));
            //! Acknoledge
            __channel->Access<bool>(toOffseteAddress(DONE),true);
            onUpdate();
            break;
        case ACTION_CREATE:
            onInsert();
            __channel->Access<MODBUS_U_LONG>(toOffseteAddress(MATERIAL_ID),__materialId);
            __channel->Access<bool>(toOffseteAddress(DONE),true);
            break;
        case ACTION_QUERY:
            //!Write data
            //! find record by material id
            onQuery();
            __channel->Access<CellDataBlock>(toOffseteAddress(BLOCK_DATA),static_cast<CellDataBlock>(__adb));
            __channel->Access<bool>(toOffseteAddress(DONE),true);
            break;
        default:
            break;
        }

        //! Acknoledge
        //__channel->Access<bool>(toOffseteAddress(DONE),true);
        //__pollCyclic=0;//speed up
    });

    //!
    //! s1
    ValueTransition* actOff = new ValueTransition(toOffseteAddress(ACT),ValueTransition::BIT_STATE_OFF);
    actOff->setTargetState(s0);
    s1->addTransition(actOff);
    connect(s1,&QState::exited,[this](){

        //!OK/NG Counting
        m_currentGrade = static_cast<Grade>(__adb.Value(m_index_grade1).toInt() && __adb.Value(m_index_grade2).toInt());
        switch (m_currentGrade) {
        case OK:
        case BYPASS:
            m_okCounter+=1;
            //! Main grade
            m_currentGrade = static_cast<Grade>(__adb.Value(m_index_grade1).toInt());
            break;
        case NG:
            m_ngCounter+=1;
            m_currentGrade = Grade::NG;
            break;
        default:
            break;
        }


       emit dataUpdated();
        __channel->Access<bool>(toOffseteAddress(DONE),false); //lead
       qDebug() << QString("%1,procedure elapsed,%2").arg(__slotIndex).arg(__procedureTimer.elapsed());
    });

    //! On reply
    connect(__channel,&ModbusChannel::readReply,this,&ControllerMaterialTransfer::onReply);
    //!Ready
    setInitialState(s0);
    start();

}

ControllerMaterialTransfer::~ControllerMaterialTransfer()
{

}

ModbusDriverAddress ControllerMaterialTransfer::toOffseteAddress(int base)
{
    ModbusDriverAddress __address(base + __slotIndex * 256);
    __address.setChannel(__channelIndex);
    return __address;
}

void ControllerMaterialTransfer::onReply()
{
    if(__connectionEngaged && !__channel->Access<bool>(toOffseteAddress(DB_ENGAGED)))
        qDebug() << QString("Material:%1 , connectionEngaged:Off").arg(__slotIndex);
    else if(!__connectionEngaged && __channel->Access<bool>(toOffseteAddress(DB_ENGAGED)))
        qDebug() << QString("Material:%1 , connectionEngaged:On").arg(__slotIndex);

    __connectionEngaged = __channel->Access<bool>(toOffseteAddress(DB_ENGAGED));

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

    for(int i=0;i<__record.count();i++)
    {
        __record.setGenerated(i,true); //all fields would be autogenerated
        __record.value(i) = QVariant::fromValue(nullptr);
    }

    //! insert into last row
    if(!__table->insertRecord(-1,__record))
    {
        qDebug() << QString("Insert record error , %1").arg(__slotIndex);
        qDebug() << __table->tableName();
        qDebug() << __table->lastError().text();
    }

    //! Carries largest id
    __table->setFilter(QString("%2 = (SELECT MAX(%2)  FROM %1)")
                       .arg(__table->tableName())
                       .arg(QVariant::fromValue(SlotBlock::ID).toString()));
    __table->select();//re-select

    int __rowCount = __table->rowCount();

    __record = __table->record(0); //access last row
    __materialId = __record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();

    if(!__table->database().commit())
        qDebug() << QString("database commit failed onInsert");
    qDebug() << QString("%1,onInsert elapsed,%2,%3").arg(__slotIndex).arg(__timer.elapsed()).arg(__materialId);


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
    qDebug() << QString("%1,onQuery elapsed,%2,%3").arg(__slotIndex).arg(__timer.elapsed()).arg(__materialId);
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

    qDebug() << QString("%1,onUpdate elapsed,%2,%3").arg(__slotIndex).arg(__timer.elapsed()).arg(__materialId);
}

void ControllerMaterialTransfer::onAboutToLeave()
{
    __channel->Access<bool>(toOffseteAddress(DB_ENGAGED),false);
    //__channel->beginAccess<bool>(toOffseteAddress(DB_ENGAGED)); //double-check
    qDebug() << QString("Material:%1 , onAboutToLeave").arg(__slotIndex);
}

QSqlDatabase ControllerMaterialTransfer::__database;
QString ControllerMaterialTransfer::__databaseName = "material.db";

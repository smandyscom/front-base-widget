#include "controllermaterialtransfer.h"

ControllerMaterialTransfer::ControllerMaterialTransfer(QObject *parent) : QObject(parent)
{
    //! Channel initialize
    __channel = ModbusChannel::Instance();

    //! Very first shot
    __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(WORD_OUT));
    //! DB engaged,
    //!
    QState* s0 = new QState(this);
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    __stateMap[WAIT_ACT_ON] = s0;
    __stateMap[SYNC] = s1;
    __stateMap[WAIT_ACT_OFF] = s2;
    //! Common
    foreach (QState* var, __stateMap.values()) {
        connect(var,&QState::entered,[=](){
           __currentState = __stateMap.key(qobject_cast<QState*>(sender()));
        });
    }
    //!
    //! s0
    ValueTransition* actOn = new ValueTransition(toOffseteAaddress(ACT),ValueTransition::BIT_STATE_ON);
    actOn->setTargetState(s1);
    s0->addTransition(actOn);
    connect(s0,&QState::exited,[=](){
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
    connect(s1,&QState::exited,[=]()
    {
        __slotMaterialId = __channel->Access<MODBUS_U_WORD>(toOffseteAaddress(MATERIAL_ID));
        __slotIsValid = __channel->Access<bool>(toOffseteAaddress(IS_VALID));
        __slotRequest = __channel->Access<SyncRequests>(toOffseteAaddress(SYNC_ACTION));

        //! According sync request perform DB manipulation
        switch (__slotRequest) {
        case ACTION_UPDATE_HEADER:
            //! slot held ID/IS_VALID updated only
            break;
        case ACTION_UPDATE_BLOCK:
            //! write into data-base
            __channel->beginAccess<CellDataBlock>(toOffseteAaddress(DATA));
            break;
        case ACTION_CREATE:
            //!insert record on table
            __table->insertRecord(__table->rowCount(),__table->record());
            __slotMaterialId = __table->record(__table->rowCount()-1)
                    .value(QVariant::fromValue(SlotBlock::ID).toString());

            __channel->Access<MODBUS_U_LONG>(toOffseteAaddress(MATERIAL_ID),
                                             __slotMaterialId);
            break;
        case ACTION_QUERY:
            //!Write data
            //! find record by material id
            __channel->Access<CellDataBlock>(toOffseteAaddress(DATA),
                                             static_cast<CellDataBlock>(
                                                 __adpator->Record(__slotMaterialId,
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
    connect(s2,&QState::exited,[=](){

        switch (__slotRequest) {
        case ACTION_UPDATE_BLOCK:
            //! Write in data base
            __adpator->Record(__slotMaterialId,
                              __channel->Access<SlotDataBlock>(toOffseteAaddress(DATA)),
                              AbstractSqlTableAdpater::KEY_NAMED_KEY,
                              QVariant::fromValue(SlotBlock::ID));
            break;
        default:
            break;
        }

        //! Rewind
       __channel->Access<bool>(toOffseteAaddress(DONE),false);
    });

    //!Ready
    setInitialState(s0);
    start();
}

ModbusDriverAddress ControllerMaterialTransfer::toOffseteAaddress(int base)
{
    return ModbusDriverAddress(base + __slotOffset * 256);
}

void ControllerMaterialTransfer::onReply(UpdateEvent *event)
{
    switch (event->address) {
    case toOffseteAaddress(WORD_OUT):
        QTimer::singleShot(0,[=](){
           __channel->beginAccess<MODBUS_U_WORD>(toOffseteAaddress(WORD_OUT));
        });
        break;
    default:
        break;
    }
}

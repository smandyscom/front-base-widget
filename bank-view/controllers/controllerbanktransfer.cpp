
#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(quint8 clientId, quint16 baseOffset, int interval=100, QObject *parent) :
    ControllerManualMode(clientId,baseOffset,interval,parent)
{
    //setup
//    __commitOption.Selection(CommitBlock::SELECTION_COMMAND_BLOCK);
//    __controller = ControllerManualMode::Instance();

//    connect(this,&ControllerBankTransfer::dataTransfered,[=](){
//       //! cut link after done
//       disconnect(__controller,SIGNAL(operationReady()),this,SLOT(onControllerOperationReady()));
//    });
}

void ControllerBankTransfer::Adaptor(ManualModeDataBlock::Categrories key,AbstractSqlTableAdpater* value)
{
    m_adaptors[key] = value;
    //! Sense data changed and put task
    connect(value->Model(),&QSqlTableModel::dataChanged,this,&ControllerBankTransfer::onDataChanged);
}

//!
//! \brief ControllerBankTransfer::onTransferData
//! First time raising transfering operation
//void ControllerBankTransfer::onTransferData()
//{
//    /*if(__commitOption.Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
//            __commitOption.Mode() != CommitBlock::MODE_UPLOAD_DATA_BLOCK)
//        return;*/ // invalid mode

//    if(__tasksQueue.isEmpty())
//    {
//        emit dataTransfered();
//        return;
//    }

//    //! connect before operation
//    connect(__controller,SIGNAL(operationReady()),this,SLOT(onOperationReady()));
//     //!Raise asynchrons operation
//    transfer();
//}

//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
//void ControllerBankTransfer::onOperationReady()
//{
////    if(__controller->CommitOption().Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
////            __controller->CommitOption().Mode()!=CommitBlock::MODE_UPLOAD_DATA_BLOCK)
////        return; //ignored

//    TransferTask __task = __tasksQueue.dequeue();

//    AbstractDataBlock __temp;
//    *static_cast<CellDataBlock*>(&__temp) = __controller->DataBlock<CellDataBlock>().value<CellDataBlock>();
//    __adaptorMap[__task.first]->
//            Record(__task.second,
//                   __temp);

//    if(__tasksQueue.isEmpty())
//    {
//        emit dataTransfered();
//        return; //no next trigger
//    }

//    //! Raise next operation
//    transfer();
//}

//void ControllerBankTransfer::transfer()
//{
//    //! Following task instruction
//    TransferTask m_task = m_tasksQueue.head();
////    __commitOption.Selection(__task.first);
////    __commitOption.Index(__task.second);
////    //! Setup option
////    __controller->CommitOption(__commitOption);
////    //! Setup data pack
////    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>(__adaptorMap[__task.first]->Record(__task.second)))); //Write-in anyway

//    emit dataTransfering(m_tasksQueue.head());

////    QtConcurrent::run([=](){
////        //! Wait until controller comes to right state
////        while (__controller->CurrentState()!=ControllerManualMode::STATE_IDLE) {}
////        //! raise state machine to work
////        emit __controller->operationTriggered();
////    });

//    //! raise state machine to work
////    emit __controller->operationTriggered();

//}

//!
//! \brief ControllerBankTransfer::plcReady
//! Iteration begin
void ControllerBankTransfer::plcReady()
{
    switch (m_mode) {
    case ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK:
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
    {
        if(!m_tasksQueue.isEmpty())
        {
            //! Auto started
            m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_1_RUN),true);
        }
        break;
    }
    default:
        break;
    }
}


void ControllerBankTransfer::runOn()
{
    m_mode = qobject_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::COMMIT_MODE).value<ManualModeDataBlock::Mode>();

    switch (m_mode) {
        //! Once triggered as Transfer mode
        case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
        case ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK:
        {
            //! Vanish one
            TransferTask task = m_tasksQueue.dequeue();
            //! By task queue to write-in
            m_categrory = task.first;
            m_index = task.second;
            //! Write-in commit categrory and index
            m_channel->Access(toAddressMode(ManualModeDataBlock::COMMIT_CATEGRORY),QVariant::fromValue(m_categrory).value<MODBUS_U_WORD>());
            m_channel->Access(toAddressMode(ManualModeDataBlock::COMMIT_DEVICE_INDEX),QVariant::fromValue(m_index).value<MODBUS_U_WORD>());
            break;
        }
        default:
            break;
    }

}

//!
//! \brief ControllerBankTransfer::onOperationTrigger
//! After RUN
void ControllerBankTransfer::doneOn()
{   
    switch (m_mode) {
    //! Once triggered as Transfer mode
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
    {
        m_channel->BeginRead(toAddressMode(ManualModeDataBlock::DATA_BLOCK_HEAD),QVariant::fromValue(CellDataBlock()));
        break;
    }
    case ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK:
    {
        //! Write
        CellDataBlock* data =
                reinterpret_cast<CellDataBlock*>(m_adaptors[m_categrory]->Record(m_index).Anchor());
        m_channel->Access(toAddressMode(ManualModeDataBlock::DATA_BLOCK_HEAD),QVariant::fromValue(*data));

        break;
    }
    default:
        break;
    }


    //!
    //! \brief ControllerManualMode::doneOn
    //! Run off
    ControllerManualMode::doneOn();
}
//!
//! \brief ControllerBankTransfer::doneOff
//! Once task queue not empty ,
//! Re-initiate Run
void ControllerBankTransfer::doneOff()
{
    switch (m_mode) {
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
    {
        //! Write back to Database
        CellDataBlock data =
                qobject_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::DATA_BLOCK_HEAD).value<CellDataBlock>();
        m_adaptors[m_categrory]->Record(m_index,AbstractDataBlock(reinterpret_cast<MODBUS_U_WORD*>(&data)));
    }
    default:
        break;
    }

    ControllerManualMode::doneOff();
}


void ControllerBankTransfer::onDataChanged(const QModelIndex &topLeft,
                                           const QModelIndex &bottomRight,
                                           const QVector<int> &roles)
{
    AbstractSqlTableAdpater* adaptor = nullptr;
    //! Search for Adaptor
    foreach (AbstractSqlTableAdpater* var, m_adaptors.values()) {
        if(var->Model()==sender())
        {
            adaptor = var;
            break;
        }
    }
    //!
    TransferTask task;
    task.first = m_adaptors.key(adaptor) ;
    //! Turns into absolute row index
    task.second =  adaptor->Model()->record( topLeft.row())
             .value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString())
             .toInt();

    m_tasksQueue.enqueue(task);
}

//!
//! \brief ControllerBankTransfer::m_operator_propertyChanged
//! \param key
//! \param value
//! Trigger transfer task
void ControllerBankTransfer::m_operator_propertyChanged(QVariant key, QVariant value)
{
    //! Prepare
    switch (key.toUInt()) {
    case ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE:
        //! Clear task queue (Write
        break;
    case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
        //! Populating
//        for(int i=0;i<m_adaptors[task.first]->Model()->rowCount();i++)
//            m_tasksQueue.enqueue(TransferTask(task.first,i));
//        break;
    case ManualModeDataBlock::BATCH_ALL_READ_MODE:
        break;
    default:
//        //! Singal mode (Positive index
//        //! Find if repeated
//        if(!m_tasksQueue.contains(task))
//            m_tasksQueue.enqueue(task);
        break;
    }
    //!Action
    switch (key.toUInt()) {
    case ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE:
    case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
    case ManualModeDataBlock::BATCH_ALL_READ_MODE:
        //! trigger operation
        if(m_currentState = ManualState::STATE_PLC_READY)
            m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_1_RUN),true);
        setProperty(key.toString().toStdString().c_str(),false); //reset property
        break;
    default:
        ControllerManualMode::m_operator_propertyChanged(key,value);
        break;
    }
}



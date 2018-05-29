
#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(QObject *parent) :
    QObject(parent)
{
    //setup
    __commitOption.Selection(CommitBlock::SELECTION_COMMAND_BLOCK);
    __controller = ControllerManualMode::Instance();

    connect(this,&ControllerBankTransfer::dataTransfered,[=](){
       //! cut link after done
       disconnect(__controller,SIGNAL(operationPerformed()),this,SLOT(onControllerOperationPerformed()));
    });
}

void ControllerBankTransfer::onTransferData()
{
    if(__commitOption.Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
            __commitOption.Mode() != CommitBlock::MODE_UPLOAD_DATA_BLOCK)
        return; // invalid mode

    //! connect before operation
    connect(__controller,SIGNAL(operationPerformed()),this,SLOT(onControllerOperationPerformed()));

    if(__tasksQueue.isEmpty())
    {
        emit dataTransfered();
        return;
    }

     //!Raise asynchrons operation
    QtConcurrent::run(this,&ControllerBankTransfer::transfer);
    emit dataTransfering(__tasksQueue.head());
}

//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
void ControllerBankTransfer::onControllerOperationPerformed()
{
    if(__controller->CommitOption().Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
            __controller->CommitOption().Mode()!=CommitBlock::MODE_UPLOAD_DATA_BLOCK)
        return; //ignored

    TransferTask __task = __tasksQueue.dequeue();

    AbstractDataBlock __temp;
    *static_cast<CellDataBlock*>(&__temp) = __controller->DataBlock<CellDataBlock>().value<CellDataBlock>();
    __adaptorMap[__task.first]->
            Record(__task.second,
                   __temp);

    if(__tasksQueue.isEmpty())
    {
        emit dataTransfered();
        return; //no next trigger
    }

    //! Raise next operation
    QtConcurrent::run(this,&ControllerBankTransfer::transfer);
    emit dataTransfering(__tasksQueue.head());
}

void ControllerBankTransfer::transfer()
{
    //! Following task instruction
    TransferTask __task = __tasksQueue.head();
    __commitOption.Selection(__task.first);
    __commitOption.Index(__task.second);
    //! Setup option
    __controller->CommitOption(__commitOption);
    //! Setup data pack
    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>(__adaptorMap[__task.first]->Record(__task.second)))); //Write-in anyway
    //! Wait until controller comes to right state
    while (__controller->CurrentState()!=ControllerManualMode::STATE_IDLE) {}
    emit __controller->operationTriggered();
}

void ControllerBankTransfer::PutTask(TransferTask task)
{
    switch (task.second) {
    case BATCH_ALL_MODE:
    {
        //! Populating
        for(int i=0;i<__adaptorMap[task.first]->Model()->rowCount();i++)
            __tasksQueue.enqueue(TransferTask(task.first,i));
        break;
    }
    case BATCH_PRESCHEDUALED_MODE:
        //! Do nothing , let following procedure wipe out all things in queue
        if(__tasksQueue.isEmpty())
            return; //nothing to do
        break;
    default:
        //! Singal mode (Positive index
        //! Find if repeated
        if(!__tasksQueue.contains(task))
            __tasksQueue.enqueue(task);
        break;
    }
}

QMap<CommitBlock::CommitDataBlockSelection,AbstractSqlTableAdpater*> ControllerBankTransfer::__adaptorMap;

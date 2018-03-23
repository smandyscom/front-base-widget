#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(TableModelCommandBlock *model, QObject *parent) :
    QObject(parent),
    __model(model)
{
    //setup
    __commitOption.Selection(CommitBlock::SELECTION_COMMAND_BLOCK);

    __controller = ControllerManualMode::Instance();
    connect(__controller,SIGNAL(operationPerformed()),this,SLOT(onOperationPerformed()));
}

void ControllerBankTransfer::onTransferData(CommitBlock::CommitMode mode, int rowIndex)
{
    if(mode==CommitBlock::MODE_COMMAND_BLOCK)
        return; // invalid
    __commitOption.Mode(mode);
    if(rowIndex==-1)
    {
        __currentIndex = 0;
        __goal = __model->rowCount(); //batch mode activated
    }
    else
    {
        __currentIndex = rowIndex;
        __goal = __currentIndex+1; //single mode
    }

    //!Raise operation
    QtConcurrent::run(this,&ControllerBankTransfer::transfer);
}

//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
void ControllerBankTransfer::onOperationPerformed()
{
    if(__controller->CommitOption().Mode()==CommitBlock::MODE_COMMAND_BLOCK)
        return; //ignored

    //__model->RowRecord(__currentIndex,__controller->DataBlock<AbstractDataBlock>()); // read-out anyway (would be override in commit mode
    __currentIndex = __controller->CommitOption().Index()+1; //follow the current index
    //! Raise next operation if any
    if(__currentIndex < __goal)
    {
        QtConcurrent::run(this,&ControllerBankTransfer::transfer);
    }
    else
    {
        emit dataTransfered();
        return; //no next trigger
    }
}

void ControllerBankTransfer::transfer()
{
    __commitOption.Index(__currentIndex);
    __controller->CommitOption(__commitOption);
    __controller->DataBlock(__model->RowRecord(__currentIndex)); //Write-in anyway
    //! Wait until controller comes to right state
    while (__controller->CurrentState()!=ControllerManualMode::STATE_IDLE) {}
    emit __controller->operationTriggered();
}

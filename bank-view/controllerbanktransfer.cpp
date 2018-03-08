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
    __commitOption.Index(__currentIndex);
    __controller->CommitOption(__commitOption);
    __controller->CommandBlock(__model->Row(__currentIndex)); //Write-in anyway (would be override in update mode
    emit __controller->operationTriggered();
}

//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
void ControllerBankTransfer::onOperationPerformed()
{
    __model->Row(__currentIndex,__controller->CommandBlock()); // read-out anyway (would be override in commit mode
    __currentIndex++;
    //! Raise next operation if any
    if(__currentIndex < __goal)
    {
        __commitOption.Index(__currentIndex);
        __controller->CommitOption(__commitOption);
        __controller->CommandBlock(__model->Row(__currentIndex)); //Write-in anyway
        emit __controller->operationTriggered();
    }
    else
        return; //no next trigger
}

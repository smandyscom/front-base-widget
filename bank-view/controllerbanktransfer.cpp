#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(QObject *parent) :
    QObject(parent)
{
    //setup
    __commitOption.Selection(CommitBlock::SELECTION_COMMAND_BLOCK);
    __controller = ControllerManualMode::Instance();

    connect(__controller,SIGNAL(operationPerformed()),this,SLOT(onControllerOperationPerformed()));

    __currentIndex = !PROCESSING;
}

void ControllerBankTransfer::onTransferData(int rowIndex)
{
    if(__commitOption.Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
            __commitOption.Mode() != CommitBlock::MODE_UPLOAD_DATA_BLOCK)
        return; // invalid mode

    if(rowIndex==BATCH_MODE)
    {
        __currentIndex = 0;
        __goal = __adaptor->Model()->rowCount(); //batch mode activated
    }
    else
    {
        __currentIndex = rowIndex;
        __goal = __currentIndex+1; //single mode
    }

    //!Raise asynchrons operation
    QtConcurrent::run(this,&ControllerBankTransfer::transfer);
}

//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
void ControllerBankTransfer::onControllerOperationPerformed()
{
    if(__controller->CommitOption().Mode()!=CommitBlock::MODE_DOWNLOAD_DATA_BLOCK &&
            __controller->CommitOption().Mode()!=CommitBlock::MODE_UPLOAD_DATA_BLOCK)
        return; //ignored

    __adaptor->Record(__currentIndex,__controller->DataBlock<AbstractDataBlock>().value<AbstractDataBlock>());
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

    emit dataTransfering();
}

void ControllerBankTransfer::transfer()
{
    __commitOption.Index(__currentIndex);
    __controller->CommitOption(__commitOption);
    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>( __adaptor->Record(__currentIndex)))); //Write-in anyway
    //! Wait until controller comes to right state
    while (__controller->CurrentState()!=ControllerManualMode::STATE_IDLE) {}
    emit __controller->operationTriggered();
}

ControllerBankTransfer* ControllerBankTransfer::Instance()
{
    if(__instance==nullptr)
        __instance = new ControllerBankTransfer();
    return __instance;
}

ControllerBankTransfer* ControllerBankTransfer::__instance = nullptr;

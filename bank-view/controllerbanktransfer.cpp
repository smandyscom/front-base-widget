#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(QObject *parent) :
    QObject(parent)
{
    //setup
    __commitOption.Selection(CommitBlock::SELECTION_COMMAND_BLOCK);

    __goalCount = 128;
}

void ControllerBankTransfer::onUpdatedFromPlc()
{
    //!
    //! 1.Commit request
    //! 2.Wait until operationPerformed (SIGNAL
    __commitOption.Mode(CommitBlock::MODE_UPLOAD);
    onOperationPerformed();//raise first shot
}
void ControllerBankTransfer::onCommitToPlc()
{
    __commitOption.Mode(CommitBlock::MODE_DOWNLOAD);
    onOperationPerformed();//raise first shot
}
//!
//! \brief ControllerBankTransfer::onOperationPerformed
//! Would iteratlly perform opertion until index reached
void ControllerBankTransfer::onOperationPerformed()
{
    //! Turns QVariant data to Mode data
    //! TODOS

    if(__currentIndex!=-1)
    {
        ExtendedCommandBlock generic = __controller->CommandBlock();

        switch (__commitOption.Mode()) {
        case CommitBlock::MODE_UPLOAD:
        {
           //__commandBankTable->setData(__commandBankTable->index(__currentIndex,JunctionBankDatabase::CBT_AXIS_ID),generic.ObjectId());
           //__commandBankTable->setData(__commandBankTable->index(__currentIndex,JunctionBankDatabase::CBT_AXIS_ID),generic.CommandType());
            break;
        }
        case CommitBlock::MODE_DOWNLOAD:
        {
            //generic.ObjectId(__commandBankTable->data(__commandBankTable->index(__currentIndex,JunctionBankDatabase::CBT_AXIS_ID)).value<MODBUS_WORD>());
            //generic.CommandType(__commandBankTable->data(__commandBankTable->index(__currentIndex,JunctionBankDatabase::CBT_AXIS_ID)).value<MODBUS_WORD>());
            break;
        }
        default:
            break;
        }
    }

    //! Raise next operation if any
    __currentIndex++;
    __commitOption.Index(__currentIndex);
    __controller->CommitOption(__commitOption);
    //!
    if(__currentIndex < __goalCount)
        emit __controller->operationTriggered();
    else
        __currentIndex = -1; //reset
}

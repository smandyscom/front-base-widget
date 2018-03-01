#include "controllerbankmanager.h"

ControllerBankManager::ControllerBankManager(QObject *parent) : QObject(parent)
{

}

void ControllerBankManager::onUpdatedFromPlc()
{
    //!
    //! 1.Commit request
    //! 2.Wait until operationPerformed (SIGNAL
    controller->setCommitMode(CommitBlock::MODE_UPLOAD);
    controller->setCommitSelection(CommitBlock::SELECTION_COMMAND_BLOCK);
    onOperationPerformed();//raise first shot
}
void ControllerBankManager::onCommitToPlc()
{
    controller->setCommitMode(CommitBlock::MODE_DOWNLOAD);
    controller->setCommitSelection(CommitBlock::SELECTION_COMMAND_BLOCK);
    onOperationPerformed();//raise first shot
}

void ControllerBankManager::onOperationPerformed()
{
    //! Turns QVariant data to Mode data
    //! TODOS

    if(currentIndex!=-1)
    {
        ExtendedCommandBlock* generic = controller->CommandBlock();

        switch (control) {
        case CommitBlock::MODE_UPLOAD:
        {


           __commandBankTable->setData(__commandBankTable->index(currentIndex,JunctionBankDatabase::CBT_AXIS_ID),generic->ObjectId());
           __commandBankTable->setData(__commandBankTable->index(currentIndex,JunctionBankDatabase::CBT_AXIS_ID),generic->CommandType());
            break;
        }
        case CommitBlock::MODE_DOWNLOAD:
        {
            generic->ObjectId(__commandBankTable->data(__commandBankTable->index(currentIndex,JunctionBankDatabase::CBT_AXIS_ID)));
            generic->CommandType(__commandBankTable->data(__commandBankTable->index(currentIndex,JunctionBankDatabase::CBT_AXIS_ID)));
            break;
        }
        default:
            break;
        }
    }

    //! Raise next operation if any
    currentIndex++;
    controller->setCommitIndex(currentIndex);
    if(currentIndex > 256)
        currentIndex = -1; //reset
    else
        emit controller->operationTriggered();
}

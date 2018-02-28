#include "frontbankmanager.h"
#include "ui_frontbankmanager.h"

FrontBankManager::FrontBankManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontBankManager)
{
    ui->setupUi(this);
}

FrontBankManager::~FrontBankManager()
{
    delete ui;
}

void FrontBankManager::onUpdatedFromPlc()
{
    controller->setCommitMode(CommitBlock::MODE_UPLOAD);
    controller->setCommitSelection(CommitBlock::SELECTION_COMMAND_BLOCK);

    //! QVariant to ModelRow
    //!
    for(int i=0;i<1024;i++)
    {
        controller->setCommitIndex(i);
        emit controller->operationTriggered();

        //! wait until operationPerformed()
        //! Turns QVariant data to Mode data
        model->setData(model->index(i,0),);
    }
}

#include "frontbanktransfer.h"
#include "ui_frontbanktransfer.h"
#include <QList>

FrontBankTransfer::FrontBankTransfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontBankTransfer)
{
    ui->setupUi(this);
    __controllerTransfer = new ControllerBankTransfer(this);

    __titlePrefix = ui->groupBox->title();

    //!Push button
    QList<QPushButton*> __list = {
        ui->pushButtonUpload,
        ui->pushButtonDownload
    };

    foreach (QPushButton* var, __list) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onButtonClicked()));
    }

    __interlockedList = {
        ui->pushButtonDownload,
        ui->pushButtonUpload
    };
    //!Progress bar
    connect(__controllerTransfer,&ControllerBankTransfer::dataTransfering,[=](){
        ui->textBrowserProgress->setText(QString(__controllerTransfer->RestTasksCount()));
        ui->progressBar->setValue(__controllerTransfer->RestTasksCount());
    });
    connect(__controllerTransfer,&ControllerBankTransfer::dataTransfering,this,&FrontBankTransfer::onTransfering);

    //!Launch
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onScanning()));
    __timer->start();
}

void FrontBankTransfer::onButtonClicked()
{
    if(sender()==ui->pushButtonUpload)
    {
        __controllerTransfer->Direction(CommitBlock::MODE_UPLOAD_DATA_BLOCK);
    }
    else if(sender()==ui->pushButtonDownload)
    {
        __controllerTransfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
    }
    //! Configure and run
    __controllerTransfer->PutTask(TransferTask(__selection,ControllerBankTransfer::BATCH_ALL_MODE));
    __controllerTransfer->onTransferData();

    //!Setup progress bar
    ui->progressBar->setMaximum(0);
    ui->progressBar->reset();
}
void FrontBankTransfer::onScanning()
{
    //!Forbidden to control when processing
    foreach (QWidget* var, __interlockedList) {
        var->setEnabled(!__controllerTransfer->IsProcessing());
    }
}

FrontBankTransfer::~FrontBankTransfer()
{
    delete ui;
}

void FrontBankTransfer::Selection(CommitBlock::CommitDataBlockSelection value)
{
    __selection = value;

}
void FrontBankTransfer::onTransfering(TransferTask task)
{
    //! Change over current selection visual
    ui->groupBox->setTitle(QString("%1:%2,%3")
                           .arg(__titlePrefix)
                           .arg(utilities::getSqlTableSelectedRecord(ObjectTable,
                                                                     QVariant::fromValue(ID),
                                                                     QVariant::fromValue(static_cast<int>(task.first)))
                                .value(QVariant::fromValue(zh_TW).toString())
                                .toString())
                           .arg(task.second));
}

QSqlTableModel* FrontBankTransfer::ObjectTable = nullptr;

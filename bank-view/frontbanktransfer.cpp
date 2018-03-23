#include "frontbanktransfer.h"
#include "ui_frontbanktransfer.h"
#include <QList>

FrontBankTransfer::FrontBankTransfer(CommitBlock::CommitDataBlockSelection selection, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontBankTransfer),
    __selection(selection)
{
    ui->setupUi(this);
    __transfer = ControllerBankTransfer::Instance();

    //!Push button
    QList<QPushButton*> __list = {ui->pushButtonUpload,
                                  ui->pushButtonDownload};

    foreach (QPushButton* var, __list) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onButtonClicked()));
    }

    __interlockedList = {
        ui->pushButtonDownload,
        ui->pushButtonUpload
    };
    //!Progress bar
    connect(__transfer,&ControllerBankTransfer::dataTransfering,[=](){
        ui->textBrowserProgress->setText(tr("%1/%2").arg(__transfer->CurrentIndex()+1).arg(__transfer->Goal()));
        //TODO , Q_ENUM not Declared
        //ui->textBrowserDataBlockSelection->setText(QVariant::fromValue(__controller->DataBlockSelection()).value<QString>());
        ui->textBrowserDataBlockSelection->setText(QString::number(__transfer->DataBlockSelection()));
        ui->progressBar->setValue(__transfer->CurrentIndex()+1);
    });

    //!Launch
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onScanning()));
    __timer->start(100);
}

void FrontBankTransfer::onButtonClicked()
{
    if(sender()==ui->pushButtonUpload)
    {
        __transfer->Direction(CommitBlock::MODE_UPLOAD_DATA_BLOCK);
    }
    else if(sender()==ui->pushButtonDownload)
    {
        __transfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
    }
    __transfer->DataBlockSelection(__selection);
    __transfer->onTransferData();

    //!Setup progress bar
    ui->progressBar->setMaximum(__transfer->Goal());
    ui->progressBar->reset();
}
void FrontBankTransfer::onScanning()
{
    //!Forbidden to control when processing
    foreach (QWidget* var, __interlockedList) {
        var->setEnabled(!__transfer->IsProcessing());
    }
}

FrontBankTransfer::~FrontBankTransfer()
{
    delete ui;
}

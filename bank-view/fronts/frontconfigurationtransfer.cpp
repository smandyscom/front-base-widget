#include "frontconfigurationtransfer.h"
#include "ui_frontconfigurationtransfer.h"

FrontConfigurationTransfer::FrontConfigurationTransfer(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontConfigurationTransfer)
{
    ui->setupUi(this);
    //!
//    __objectTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);
//    utilities::linkQComboBoxAndModel(ui->comboBoxItemSelection,
//                                     __objectTable,
//                                     QVariant::fromValue(zh_TW));
//    connect(ui->comboBoxItemSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(onObjectTypeSelected()));
////    connect(ui->pushDownloadferAll,SIGNAL(clicked(bool)),this,SLOT(onTransferAll()));
//    ui->comboBoxItemSelection->setCurrentIndex(0);
//    //!
////    __fbt = new FrontBankTransfer(ui->widgetTransfer);
////    __fsf = new FrontSingleFilter(ui->widgetFilter);
//    //!
////    __controllerTransfer = __fbt->ControllerTransfer();
//    //!
//    __regionTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION);
//    __axisTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
//    //!
//    __disableList[AUTH::ROLE_ENGINEER].append(this);
}

FrontConfigurationTransfer::~FrontConfigurationTransfer()
{
    delete ui;
}

void FrontConfigurationTransfer::Setup(QSqlTableModel* commandTable,
            QSqlTableModel* axisTable,
           QSqlTableModel* cylinderTable,
           QSqlTableModel* unitTable,
           QSqlTableModel* signalTable,
           QSqlTableModel* objectTable,
           QSqlTableModel* regionTable)
{
    m_commandTable = commandTable;
    m_axisTable = axisTable;
    m_cylinderTable = cylinderTable;
    m_unitTable = unitTable;
    m_signalTable = signalTable;
}

void FrontConfigurationTransfer::onObjectTypeSelected()
{
    //!Change over Table view
//    int objectId =
//            __objectTable->record(ui->comboBoxItemSelection->currentIndex()).value(QVariant::fromValue(ID).toString()).toInt();
////    ui->tableView->setModel(ControllerBankTransfer::Adaptor(CommitBlock::CommitCategrories(objectId))->Model());
//    //!Change over Bank transfer
////    __fbt->Selection(CommitBlock::CommitCategrories(objectId));
//    //!Change over filter
//    __fsf->DataTable(reinterpret_cast<QSqlTableModel*>(ui->tableView->model()));
//    if(objectId==CommitBlock::SELECTION_COMMAND_BLOCK)
//    {
//        __fsf->PrimaryTable(__axisTable);
//        __fsf->DataKey(QString("AXIS_ID"));
//    }
//    else
//    {
//        __fsf->PrimaryTable(__regionTable);
//        __fsf->DataKey(QString("REGION"));
//    }

}

void FrontConfigurationTransfer::onTransferAll()
{
//    QList<CommitBlock::CommitCategrories> __list = {
//        CommitBlock::SELECTION_AXIS,
//        CommitBlock::SELECTION_CYLINDER,
//        CommitBlock::SELECTION_SIGNAL,
//        CommitBlock::SELECTION_COMMAND_BLOCK,
//        CommitBlock::SELECTION_UNIT
//    };
//    foreach (CommitBlock::CommitCategrories var, __list) {
//        __controllerTransfer->PutTask(TransferTask(var,ControllerBankTransfer::BATCH_ALL_MODE));
//    }
//    __controllerTransfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
//    __controllerTransfer->onTransferData();
}

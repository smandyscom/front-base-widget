#include "frontconfigurationtransfer.h"
#include "ui_frontconfigurationtransfer.h"

FrontConfigurationTransfer::FrontConfigurationTransfer(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontConfigurationTransfer)
{
    ui->setupUi(this);
    //!
    QList<QPushButton*> buttons{
                ui->pushButtonDownload,
                ui->pushButtonDownloadAll,
                ui->pushButtonUpload,
                ui->pushButtonUploadAll
    };
    foreach (QPushButton* var, buttons) {
        connect(var,&QPushButton::clicked,this,&FrontConfigurationTransfer::onPushButtonClicked);
    }
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
           QSqlTableModel* objectTable)
{
    m_tables[ManualModeDataBlock::SELECTION_COMMAND_BLOCK] = commandTable;
    m_tables[ManualModeDataBlock::SELECTION_AXIS] = axisTable;
    m_tables[ManualModeDataBlock::SELECTION_CYLINDER] = cylinderTable;
    m_tables[ManualModeDataBlock::SELECTION_SIGNAL] = signalTable;
    m_tables[ManualModeDataBlock::SELECTION_UNIT] = unitTable;
    //!
    utilities::linkQComboBoxAndModel(ui->comboBoxItemSelection,
                                     objectTable,
                                     QVariant::fromValue(zh_TW));
    connect(ui->comboBoxItemSelection,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onObjectTypeSelected(int)));

    m_idColumn = objectTable->fieldIndex(QVariant::fromValue(HEADER_STRUCTURE::ID).toString());
}

void FrontConfigurationTransfer::onObjectTypeSelected(int rowIndex)
{
    //!Change over Table view
    m_currentObject =
            ui->comboBoxItemSelection->model()->index(rowIndex,m_idColumn).data().value<ManualModeDataBlock::Categrories>();
    ui->tableView->setModel(m_tables[m_currentObject]);
    //!Change over Bank transfer
//    __fbt->Selection(CommitBlock::CommitCategrories(objectId));
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

void FrontConfigurationTransfer::onPushButtonClicked()
{
	QVariant key;
	QVariant value;

    if(sender() == ui->pushButtonDownloadAll)
    {
		key = QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_WRITE_MODE);
		value = QVariant::fromValue(ManualModeDataBlock::SELECTION_ALL);
        /*m_controllers->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_WRITE_MODE).toString().toStdString().c_str(),
                                  QVariant::fromValue(ManualModeDataBlock::SELECTION_ALL));*/
    }
    else if(sender() == ui->pushButtonUploadAll)
    {
		key = QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_READ_MODE);
		value = QVariant::fromValue(ManualModeDataBlock::SELECTION_ALL);
       /* m_controllers->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_READ_MODE).toString().toStdString().c_str(),
                                  QVariant::fromValue(ManualModeDataBlock::SELECTION_ALL));*/
    }
    else if(sender() == ui->pushButtonDownload)
    {
		key = QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_WRITE_MODE);
		value = QVariant::fromValue(m_currentObject);
        /*m_controllers->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_WRITE_MODE).toString().toStdString().c_str(),
                                  QVariant::fromValue(m_currentObject));*/
    }
    else if(sender() == ui->pushButtonUpload)
    {
		key = QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_READ_MODE);
		value = QVariant::fromValue(m_currentObject);
        /*m_controllers->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_READ_MODE).toString().toStdString().c_str(),
                                  QVariant::fromValue(m_currentObject));*/
    }

	m_controller->setProperty(key.toString().toStdString().c_str(),
		value);
}

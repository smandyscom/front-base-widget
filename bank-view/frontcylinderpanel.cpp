#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontCylinderPanel),
    __cylinderTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS))
{
    ui->setupUi(this);
    //!
    __controller  = ControllerManualMode::Instance();
    //!
    __inputFields = {
        TableModelCylinder::SEN_A_1,
        TableModelCylinder::SEN_A_2,
        TableModelCylinder::SEN_A_3,
        TableModelCylinder::SEN_A_4,
        TableModelCylinder::SEN_B_1,
        TableModelCylinder::SEN_B_2,
        TableModelCylinder::SEN_B_3,
        TableModelCylinder::SEN_B_4,
                    };
    __outputFields = {
        TableModelCylinder::ACT_A_1,
        TableModelCylinder::ACT_A_2,
        TableModelCylinder::ACT_B_1,
        TableModelCylinder::ACT_B_2,
    };
    //! Loading widgets
    new FrontSingleFilter(__cylinderTable,
                          JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                          "REGION",
                          "NAME",
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_CYLINDER,
                          ui->widgetBankTransfer);
    //!
    ui->tableViewInputs->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::INPUT_ATTRIBUTES));
    ui->tableViewOutputs->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::OUTPUT_ATTRIBUTES));
    //!
    ui->tableViewCylinder->setModel(__cylinderTable);
    ui->tableViewCylinder->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCylinder->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tableViewCylinder,SIGNAL(clicked(QModelIndex)),this,SLOT(onViewSelectionChanged()));
    //!
    //! \brief connect
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonGoA,
                                        ui->pushButtonGoB};
    foreach (QPushButton* var, __buttonList) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCylinderCommandClicked()));
    }
    //! Timer
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start();
}


FrontCylinderPanel::~FrontCylinderPanel()
{
    delete ui;
}

void FrontCylinderPanel::onCylinderCommandClicked()
{
    CommitBlock __cb;
    CylinderOperationBlock __cob;

    __cb.Mode(CommitBlock::MODE_EXE_CYLINDER);

    if(sender()==ui->pushButtonGoA)
    {
        __cob.CommandsOperation(CylinderOperationBlock::OP_COMMAND_A);
    }
    else if(sender()==ui->pushButtonGoB)
    {
        __cob.CommandsOperation(CylinderOperationBlock::OP_COMMAND_B);
    }

    __controller->CommitOption(__cb);
    __controller->DataBlock(QVariant::fromValue(__cob));
    emit __controller->operationTriggered();
}

void FrontCylinderPanel::onViewSelectionChanged()
{
    QModelIndex __index = ui->tableViewCylinder->selectionModel()->selectedRows().first();
    QSqlRecord __record = __cylinderTable->record(__index.row());

    QList<QString> __inputAddress;
    QList<QString> __outputAddress;
    foreach (TableModelCylinder::Offset var, __inputFields) {
        __inputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    foreach (TableModelCylinder::Offset var, __outputFields) {
        __outputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    //! Generate filter string
    QString __inputFilterString = generateFilterString("PLC_ADDRESS",__inputAddress);
    QString __outputFilterString = generateFilterString("PLC_ADDRESS",__outputAddress);
    //filter out inputs
    qobject_cast<QSqlTableModel*>(ui->tableViewInputs->model())->setFilter(__inputFilterString);
    qobject_cast<QSqlTableModel*>(ui->tableViewOutputs->model())->setFilter(__outputFilterString);

    //! Changeover monitor index
    __currentViewIndex = __record.value(QVariant::fromValue(TableModelCylinder::CYL_ID).toString()).value<MODBUS_WORD>();
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
}

QString FrontCylinderPanel::generateFilterString(QString key, QList<QString> conditions)
{
    int i=0;
    QString result;
    while (i<conditions.count()-1) {
        if(conditions[i] != "")
            result.append(tr("%1=\'%2\' OR ").arg(key).arg(conditions[i]));
        i++;
    }
    if(conditions[conditions.count()-1] != "")
        result.append(tr("%1=\'%2\'").arg(key).arg(conditions[conditions.count()-1]));
    else
        result.append(tr("%1=\'%2\'").arg(key).arg(0));

    return result;
}
void FrontCylinderPanel::onTimerTimeout()
{
    //! Update cylinder status
    AbstractDataBlock adb = __controller->MonitorBlock();
    CylinderMonitorBlock* cmb = reinterpret_cast<CylinderMonitorBlock*>(&adb);
    //! Show
    utilities::colorChangeOver(ui->labelWarn,cmb->Bit(CylinderMonitorBlock::MOR_WARN),Qt::yellow);
    utilities::colorChangeOver(ui->labelMonA,cmb->Bit(CylinderMonitorBlock::MOR_GROUP_A),Qt::green);
    utilities::colorChangeOver(ui->labelMonB,cmb->Bit(CylinderMonitorBlock::MOR_GROUP_B),Qt::green);
    utilities::colorChangeOver(ui->labelSuppress,cmb->Bit(CylinderMonitorBlock::CTL_SUPPRESS),Qt::red);
    utilities::colorChangeOver(ui->labelTimeon,cmb->Bit(CylinderMonitorBlock::INT_TMR_ON),Qt::green);
    utilities::colorChangeOver(ui->labelDone,cmb->Bit(CylinderMonitorBlock::MOR_DONE),Qt::green);
}

void FrontCylinderPanel::showEvent(QShowEvent *event)
{
    //! Set monitoring focus
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_CYLINDER);
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
    //!
    QWidget::showEvent(event);
}

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
       SEN_A_1,
       SEN_A_2,
       SEN_A_3,
       SEN_A_4,
       SEN_B_1,
       SEN_B_2,
       SEN_B_3,
       SEN_B_4,
                    };
    __outputFields = {
        ACT_A_1,
        ACT_A_2,
        ACT_B_1,
        ACT_B_2,
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
    connect(ui->tableViewCylinder,SIGNAL(activated(QModelIndex)),this,SLOT(onViewSelectionChanged()));
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
    QVariant command  = QVariant::fromValue((sender()==ui->pushButtonGoA?
                                                static_cast<MODBUS_WORD>(CylinderOperationBlock::OP_COMMAND_A):
                                                static_cast<MODBUS_WORD>(CylinderOperationBlock::OP_COMMAND_B)));

    __cb.Mode(CommitBlock::MODE_EXE_CYLINDER);
    __cob.Value(CylinderOperationBlock::OFFSET_OPERATION_COMMAND_CACHED,command);
    __controller->CommitOption(__cb);
    __controller->DataBlock(QVariant::fromValue(__cob));
    emit __controller->operationTriggered();
}

void FrontCylinderPanel::onViewSelectionChanged()
{
    QSqlRecord __record = __cylinderTable->record(ui->tableViewCylinder->selectionModel()->selectedRows().first().row());

    QList<QString> __inputAddress;
    QList<QString> __outputAddress;
    foreach (DataBaseHeaders var, __inputFields) {
        __inputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    foreach (DataBaseHeaders var, __outputFields) {
        __outputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    //! Generate filter string
    QString __inputFilterString = generateFilterString(QVariant::fromValue(IO_ATTR_PLC_ADDRESS).toString(),__inputAddress);
    QString __outputFilterString = generateFilterString(QVariant::fromValue(IO_ATTR_PLC_ADDRESS).toString(),__outputAddress);
    //filter out inputs
    qobject_cast<QSqlTableModel*>(ui->tableViewInputs->model())->setFilter(__inputFilterString);
    qobject_cast<QSqlTableModel*>(ui->tableViewOutputs->model())->setFilter(__outputFilterString);

    //! Changeover monitor index
    __currentViewIndex = __record.value(QVariant::fromValue(CYL_ID).toString()).value<MODBUS_WORD>();
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
    CylinderMonitorBlock cmb = QVariant::fromValue(__controller->MonitorBlock()).value<CylinderMonitorBlock>();
    //! Show
    //utilities::colorChangeOver(ui->labelWarn,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_MOR_WARN).toBool(),Qt::yellow);
    //utilities::colorChangeOver(ui->labelMonA,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_MOR_GROUP_A).toBool(),Qt::green);
    //utilities::colorChangeOver(ui->labelMonB,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_MOR_GROUP_B).toBool(),Qt::green);
    //utilities::colorChangeOver(ui->labelSuppress,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_CTL_SUPPRESS).toBool(),Qt::red);
    //utilities::colorChangeOver(ui->labelTimeon,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_INT_TMR_ON).toBool(),Qt::green);
    //utilities::colorChangeOver(ui->labelDone,cmb.Value(CylinderMonitorBlock::OFFSET_MONITOR_MOR_DONE).toBool(),Qt::green);
}

void FrontCylinderPanel::showEvent(QShowEvent *event)
{
    //! Set monitoring focus
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_CYLINDER);
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
    //!
    QWidget::showEvent(event);
}

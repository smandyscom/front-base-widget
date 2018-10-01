#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontCylinderPanel),
    m_index(0)
{
    ui->setupUi(this);
    m_categrory = ManualModeDataBlock::SELECTION_CYLINDER;
    m_status = utilities::listupEnumVariant<CylinderMonitorBlock::Status>();
    //!
    //! \brief connect
    //!
    QList<QPushButton*> buttonList = {ui->pushButtonGoA,
                                        ui->pushButtonGoB};
    foreach (QPushButton* var, buttonList) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCylinderCommandClicked()));
    }
    //!
    //! \brief connect
    //! When monitor/opertion selection changed
    connect(ui->tableViewCylinder,&QTableView::clicked,this,&FrontCylinderPanel::onMonitorIndexChanged);

    //! Interlock
//    __busyInterlock = {
//        ui->pushButtonGoA,
//        ui->pushButtonGoB,
//        ui->tableViewCylinder,
//    };
    //!
//    __disableList[AUTH::ROLE_OPERATOR].append(this);

    //! Stop polling when editing
//    connect(ui->tableViewCylinder,&QTableView::doubleClicked,[=](){
//       qobject_cast<TableModelCylinderVisual*>(__cylinderTable)->onBeforeEditing();
//    });
//    connect(ui->tableViewCylinder,&QTableView::clicked,[=](){
//        qobject_cast<TableModelCylinderVisual*>(__cylinderTable)->onAfterEditing();
//    });

}


FrontCylinderPanel::~FrontCylinderPanel()
{
    delete ui;
}

void FrontCylinderPanel::Setup(QSqlTableModel* cylinderTable,
                               QSqlTableModel* regionTable,
                               QSqlTableModel* cylinderTableHeader)
{
    mainDataTable = cylinderTable;

    ui->widgetFilter->DataTable(cylinderTable);
    ui->widgetFilter->PrimaryTable(regionTable);
    //!
    ui->tableViewCylinder->setModel(cylinderTable);
    connect(ui->tableViewCylinder,&QTableView::clicked,this,&FrontCylinderPanel::onMonitorIndexChanged);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(cylinderTableHeader,
                                                     ui->tableViewCylinder,
                                                     HEADER_STRUCTURE::zh_TW,
                                                     false);
}

void FrontCylinderPanel::onCylinderCommandClicked()
{
    CylinderOperationBlock block;
    QVariant command  = QVariant::fromValue((sender()==ui->pushButtonGoA?
                                                 static_cast<MODBUS_U_WORD>(CylinderOperationBlock::OP_COMMAND_A):
                                                 static_cast<MODBUS_U_WORD>(CylinderOperationBlock::OP_COMMAND_B)));

    block.Value(CylinderOperationBlock::OFFSET_OPERATION_COMMAND_CACHED,command);

    //! Set mode
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
                              QVariant::fromValue(ManualModeDataBlock::MODE_EXE_CYLINDER));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                              QVariant::fromValue(ManualModeDataBlock::SELECTION_CYLINDER));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str(),
                              currentIndex());
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
                              QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(block.Anchor())));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),
                              true);
}

//void FrontCylinderPanel::onTimerTimeout()
//{
//    //! Interlock
//    foreach (QWidget* var, __busyInterlock) {
//        var->setEnabled(__controller->IsManualModeActiavted());
//    }
//}
void FrontCylinderPanel::dynamicPropertyChanged(int key, QVariant value)
{
    switch (key) {
    case ManualModeDataBlock::MONITOR_BLOCK_HEAD:
    {
        *(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
                value.value<CellDataBlock>(); //value assignment

        //! Self-raise Bit properties
        foreach (QVariant var, m_status) {
            setProperty(var.toString().toStdString().c_str(),m_monitorBlock.Value(var.toUInt()).toBool());
        }
        break;
    }
    default:
        break;
    }
}


int FrontCylinderPanel::currentIndex()
{
    //!Renew current index
    if(ui->tableViewCylinder->selectionModel()->hasSelection())
    {
        auto table = static_cast<QSqlTableModel*>(ui->tableViewCylinder->model());
        auto record = table->record(ui->tableViewCylinder->selectionModel()->selectedRows().first().row());
        m_index =  record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
    }
    return m_index;
}

QString FrontCylinderPanel::currentFilter() const
{
    return ui->widgetFilter->Filter();
}

void FrontCylinderPanel::showEvent(QShowEvent *event)
{
    //! Manual mode(Stop unit running
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_2_ENGAGED_MANUAL).toString().toStdString().c_str(),
                              true);
    FrontCommonManual::showEvent(event);
}
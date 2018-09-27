#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontCylinderPanel)
{
    ui->setupUi(this);

    //!
    //! \brief connect
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonGoA,
                                        ui->pushButtonGoB};
    foreach (QPushButton* var, __buttonList) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCylinderCommandClicked()));
    }
    //! Timer
//    __timer = new QTimer(this);
//    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
//    __timer->start();
    //! Map
//    __labelAddressMap[ui->labelDone] = CylinderMonitorBlock::MOR_DONE;
//    __labelAddressMap[ui->labelMonA] = CylinderMonitorBlock::MOR_GROUP_A;
//    __labelAddressMap[ui->labelMonB] = CylinderMonitorBlock::MOR_GROUP_B;
//    __labelAddressMap[ui->labelSuppress] = CylinderMonitorBlock::CTL_SUPPRESS;
//    __labelAddressMap[ui->labelTimeon] = CylinderMonitorBlock::INT_TMR_ON;
//    __labelAddressMap[ui->labelWarn] = CylinderMonitorBlock::MOR_WARN;

//    __labelColorMap[ui->labelDone] = Qt::yellow;
//    __labelColorMap[ui->labelMonA] = Qt::green;
//    __labelColorMap[ui->labelMonB] = Qt::green;
//    __labelColorMap[ui->labelSuppress] = Qt::red;
//    __labelColorMap[ui->labelTimeon] = Qt::green;
//    __labelColorMap[ui->labelWarn] = Qt::red;
    //! Interlock
//    __busyInterlock = {
//        ui->pushButtonGoA,
//        ui->pushButtonGoB,
//        ui->tableViewCylinder,
//    };
    //!
//    connect(__cylinderTable,&QSqlTableModel::dataChanged,[=](const QModelIndex &topLeft,
//            const QModelIndex &bottomRight,
//            const QVector<int> &roles = QVector<int>()){

//        if(roles.count() >0)
//            if(roles.first() == Qt::BackgroundColorRole)
//                return; //background color changed only

//        //!
//        TransferTask __task;
//        __task.first = CommitBlock::SELECTION_CYLINDER;
//        //! Turns into absolute row index
//        __task.second =  __cylinderTable->record( topLeft.row())
//                .value(QVariant::fromValue(CylinderBlock::ID).toString())
//                .toInt();
//        emit dataChanged(__task);
//    });
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

    //! Loading widgets
//    FrontSingleFilter* __fsf =  new FrontSingleFilter(ui->widgetFilter);
//    __fsf->DataTable(__cylinderTable);
//    __fsf->PrimaryTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION));

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
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str(),
                              currentIndex());
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
                              QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(block.Anchor())));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),
                              true);
}

//void FrontCylinderPanel::onViewSelectionChanged(QModelIndex index)
//{
//    QSqlRecord __record = __cylinderTable->record(index.row());
//    //! Changeover monitor index
//    __currentViewIndex =  __record.value(QVariant::fromValue(CylinderBlock::ID).toString()).value<MODBUS_S_WORD>();
//    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
//}

//void FrontCylinderPanel::onTimerTimeout()
//{
//    //! Update cylinder status
//    CylinderMonitorBlock cmb;
//    *static_cast<CellDataBlock*>(&cmb) = __controller->MonitorBlock();
//    //! Show
//    ModbusDriverAddress __address;
//    __address.setRegisterAddress(CylinderMonitorBlock::OFFSET_MONITOR_STATUS_WORD);
//    foreach (QWidget* var, __labelAddressMap.keys()) {
//        __address.setBitIndex(__labelAddressMap[var]);
//        utilities::colorChangeOver(var,cmb.Value(__address.getAddress()).toBool(),__labelColorMap[var]);
//    }
//    //! Interlock
//    foreach (QWidget* var, __busyInterlock) {
//        var->setEnabled(__controller->IsManualModeActiavted());
//    }
//}

//void FrontCylinderPanel::showEvent(QShowEvent *event)
//{
//    //! Set monitoring focus
//    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_CYLINDER);
//    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
//    //!
//    QWidget::showEvent(event);
//}

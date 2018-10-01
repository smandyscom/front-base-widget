#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontIoOverride)/*,
    __inputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_INPUTS))),
    __outputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS))),
    __signalTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS)),
    __regionTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION))*/
{
    ui->setupUi(this);
    //! Link override operation
    connect(ui->tableViewOutputs,&QTableView::clicked,this,&FrontIoOverride::onOverrideClicked);

//    //!Signal
//    ui->tableViewSignal->setModel(__signalTable);
//    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_SIGNALS),
//                                                     ui->tableViewSignal);
//    __fsf = new FrontSingleFilter(ui->widgetSignalFilter);
//    __fsf->PrimaryTable(__regionTable);
//    __fsf->DataTable(__signalTable);
//    //!
//    __controller=ControllerManualMode::Instance();
//    //!
//    __labelAddressMap[ui->labelDebouncedOn] = SignalMonitor::BIT_0_DEBOUNCED_ON;
//    __labelAddressMap[ui->labelDebouncedOff] = SignalMonitor::BIT_1_DEBOUNCED_OFF;
//    __labelAddressMap[ui->labelRawOnOff] = SignalMonitor::BIT_2_RAW;
//    //! Timer
//    __timer = new QTimer(this);
//    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
//    __timer->start();
//    //!
//    connect(ui->tableViewSignal,&QTableView::clicked,this,&FrontIoOverride::onViewSelectionChanged);
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
//!
//! \brief FrontIoOverride::Setup
//! \param input
//! \param output
//! \param region
//!
void FrontIoOverride::Setup(QSqlTableModel *input,
                            QSqlTableModel *output,
                            QSqlTableModel *region,
                            QSqlTableModel *header)
{
    ui->tableViewInputs->setModel(input);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,ui->tableViewInputs);
    ui->widgetInputFilter->PrimaryTable(region);
    ui->widgetInputFilter->DataTable(input);

    ui->tableViewOutputs->setModel(output);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,ui->tableViewOutputs);
    ui->widgetOutputFilter->PrimaryTable(region);
    ui->widgetOutputFilter->DataTable(output);
}

void FrontIoOverride::onOverrideClicked(QModelIndex index)
{
    //! Flip user-role
    QAbstractItemModel* model = const_cast<QAbstractItemModel*>(index.model());
    model->setData(index,!index.data(UserRole_MonitorOnOff).toBool(),
                           UserRole_OverrideOnOff);
}

void FrontIoOverride::onViewSelectionChanged(QModelIndex index)
{
//    QSqlRecord __record = __signalTable->record(index.row());
//    //! Changeover monitor index
//    __currentViewIndex =  __record.value(QVariant::fromValue(SignalBlock::ID).toString()).value<MODBUS_S_WORD>();
//    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
}

void FrontIoOverride::onTimerTimeout()
{
//    SignalMonitor smb;
//    *static_cast<CellDataBlock*>(&smb) = __controller->MonitorBlock();

//    foreach (QWidget* var, __labelAddressMap.keys()) {
//        utilities::colorChangeOver(var,smb.Value(__labelAddressMap[var]).toBool());
//    }
//    ui->lcdNumberOnTimer->display(smb.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_ON_COUNT).toReal());
//    ui->lcdNumberOffTimer->display(smb.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_OFF_COUNT).toReal());
}

//void FrontIoOverride::showEvent(QShowEvent *event)
//{
//    //! Set monitoring focus
//    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_SIGNAL);
//    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
//    //!
//    QWidget::showEvent(event);
//}

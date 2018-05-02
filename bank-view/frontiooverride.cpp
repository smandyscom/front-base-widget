#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontIoOverride),
    __inputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_INPUTS))),
    __outputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS))),
    __signalTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS)),
    __regionTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION))
{
    ui->setupUi(this);
    //!
    FrontSingleFilter* __fsf = new FrontSingleFilter(ui->widgetInputFilter);
    __fsf->PrimaryTable(__regionTable);
    __fsf->DataTable(__inputTable);
    //!
    __fsf = new FrontSingleFilter(ui->widgetOutputFilter);
    __fsf->PrimaryTable(__regionTable);
    __fsf->DataTable(__outputTable);
    //!
    QList<QTableView*> __views = {ui->tableViewInputs,
                                 ui->tableViewOutputs};

    ui->tableViewInputs->setModel(__inputTable);
    ui->tableViewOutputs->setModel(__outputTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
                                                     ui->tableViewInputs);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
                                                     ui->tableViewOutputs);
    //!
    connect(ui->tableViewOutputs,&QTableView::clicked,[=](QModelIndex index){
        ModbusDriverAddress address(__outputTable->record(index.row())
                                    .value(utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::HAL_ADDRESS))).toUInt());
        ModbusChannel::Instance()->Access<bool>(address,!ModbusChannel::Instance()->Access<bool>(address));
    });

    //!Signal
    ui->tableViewSignal->setModel(__signalTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_SIGNALS),
                                                     ui->tableViewSignal);
    __fsf = new FrontSingleFilter(ui->widgetSignalFilter);
    __fsf->PrimaryTable(__regionTable);
    __fsf->DataTable(__signalTable);
    //!
    __controller=ControllerManualMode::Instance();
    //!
    __labelAddressMap[ui->labelDebouncedOn] = SignalMonitor::BIT_0_DEBOUNCED_ON;
    __labelAddressMap[ui->labelDebouncedOff] = SignalMonitor::BIT_1_DEBOUNCED_OFF;
    __labelAddressMap[ui->labelRawOnOff] = SignalMonitor::BIT_2_RAW;
    //! Timer
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start();
    //!
    connect(ui->tableViewSignal,&QTableView::clicked,this,&FrontIoOverride::onViewSelectionChanged);
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}

void FrontIoOverride::onViewSelectionChanged(QModelIndex index)
{
    QSqlRecord __record = __signalTable->record(index.row());
    //! Changeover monitor index
    __currentViewIndex =  __record.value(QVariant::fromValue(SignalBlock::ID).toString()).value<MODBUS_S_WORD>();
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
}

void FrontIoOverride::onTimerTimeout()
{
    SignalMonitor smb;
    *static_cast<CellDataBlock*>(&smb) = __controller->MonitorBlock();

    foreach (QWidget* var, __labelAddressMap.keys()) {
        utilities::colorChangeOver(var,smb.Value(__labelAddressMap[var]).toBool());
    }
    ui->lcdNumberOnTimer->display(smb.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_ON_COUNT).toReal());
    ui->lcdNumberOffTimer->display(smb.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_OFF_COUNT).toReal());
}

void FrontIoOverride::showEvent(QShowEvent *event)
{
    //! Set monitoring focus
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_SIGNAL);
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
    //!
    QWidget::showEvent(event);
}

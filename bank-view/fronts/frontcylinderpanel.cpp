#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontCylinderPanel),
    m_index(0)
{
    ui->setupUi(this);

	m_auth->addDisableWidget(AUTH::ROLE_OPERATOR, this);

    //! Manual mode required
    setProperty(QVariant::fromValue(HEADER_STRUCTURE::STATE_MANUAL).toString().toStdString().c_str(),
                true);
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
    //!
    foreach (QPushButton* var, findChildren<QPushButton*>()) {
        m_widgetsPolish.append(var);
    }
    foreach (QLabel* var, findChildren<QLabel*>()) {
        m_widgetsPolish.append(var);
    }
    //! Interlock
//    __busyInterlock = {
//        ui->pushButtonGoA,
//        ui->pushButtonGoB,
//        ui->tableViewCylinder,
//    };
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
	//!Rendering region
	QSqlTableModel* dupRegion =  utilities::duplicate(regionTable);
	QList<int> regionIds;
	QString filter;
	for (size_t i = 0; i < cylinderTable->rowCount(); i++)
	{
		int regionId = cylinderTable->record(i).value(QVariant::fromValue(CylinderBlock::REGION).toString()).toInt();

		if (!regionIds.contains(regionId))
		{
			if (regionIds.count()>0)
				filter.append(QString(" OR %1='%2'").arg(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).arg(regionId));
			else
				filter.append(QString("%1='%2'").arg(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).arg(regionId));
			regionIds.append(regionId);
		}
	}
	dupRegion->setFilter(filter);
	dupRegion->select();

    ui->widgetFilter->PrimaryTable(dupRegion);
    //!
    ui->tableViewCylinder->setModel(cylinderTable);
    connect(ui->tableViewCylinder,&QTableView::clicked,this,&FrontCylinderPanel::onMonitorIndexChanged);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(cylinderTableHeader,
                                                     ui->tableViewCylinder,
                                                     HEADER_STRUCTURE::zh_TW,
                                                     true);
}

void FrontCylinderPanel::onCylinderCommandClicked()
{
    CylinderOperationBlock block;
    QVariant command  = QVariant::fromValue((sender()==ui->pushButtonGoA?
                                                 static_cast<MODBUS_U_WORD>(CylinderOperationBlock::OP_COMMAND_A):
                                                 static_cast<MODBUS_U_WORD>(CylinderOperationBlock::OP_COMMAND_B)));

    block.Value(CylinderOperationBlock::OFFSET_OPERATION_COMMAND_CACHED,command);

    //! Set mode
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE),
		QVariant::fromValue(ManualModeDataBlock::MODE_EXE_CYLINDER));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY),
		QVariant::fromValue(ManualModeDataBlock::SELECTION_CYLINDER));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX),
		currentIndex());
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD),
		QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(block.Anchor())));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN),
		true);
}

void FrontCylinderPanel::dynamicPropertyChanged(int key, QVariant value)
{
    switch (key) {
    case ManualModeDataBlock::MONITOR_BLOCK_HEAD:
    {
        *(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
                value.value<CellDataBlock>(); //value assignment

        //! Self-raise Bit properties
        foreach (QVariant var, m_status) {
			onPropertyChanged(var, m_monitorBlock.Value(var.toInt()));
        }
        //!Last command
        setProperty(QVariant::fromValue(CylinderMonitorBlock::OFFSET_MONITOR_LAST_COMMAND).toString().toStdString().c_str(),
                    m_monitorBlock.Value(CylinderMonitorBlock::OFFSET_MONITOR_LAST_COMMAND).value<CylinderMonitorBlock::Operation>());
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
        auto record = table->record(ui->tableViewCylinder->selectionModel()->selectedIndexes().first().row());
        m_index =  record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
    }
    return m_index;
}

QString FrontCylinderPanel::currentFilter() const
{
    return ui->widgetFilter->Filter();
}

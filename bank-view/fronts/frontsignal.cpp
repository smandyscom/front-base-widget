#include "frontsignal.h"

FrontSignal::FrontSignal(QWidget *parent)
	: FrontCommonManual(parent),
	m_index(0)
{
	ui.setupUi(this);

	m_auth->addDisableWidget(AUTH::ROLE_OPERATOR, this);
	//!
	setProperty(QVariant::fromValue(HEADER_STRUCTURE::STATE_MANUAL).toString().toStdString().c_str(),
		true);
	m_categrory = ManualModeDataBlock::SELECTION_SIGNAL;
	m_status = utilities::listupEnumVariant<SignalMonitor::WordOutBits>();
	foreach(QLabel* var, findChildren<QLabel*>()) {
		m_widgetsPolish.append(var);
	}
	connect(ui.tableView, &QTableView::clicked, this, &FrontSignal::currentIndex);
}

FrontSignal::~FrontSignal()
{
}

void FrontSignal::Setup(QSqlTableModel* signalTable,
	QSqlTableModel* header)
{
	mainDataTable = signalTable;
	//
	ui.tableView->setModel(signalTable);
	HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,
		ui.tableView);
}

int FrontSignal::currentIndex()
{
	//!Renew current index
	if (ui.tableView->selectionModel()->hasSelection())
	{
		auto table = static_cast<QSqlTableModel*>(ui.tableView->model());
		auto record = table->record(ui.tableView->selectionModel()->selectedIndexes().first().row());
		m_index = record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
	}
	return m_index;
}

void FrontSignal::dynamicPropertyChanged(int key, QVariant value)
{
	switch (key) {
	case ManualModeDataBlock::MONITOR_BLOCK_HEAD:
	{
		*(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
			value.value<CellDataBlock>(); //value assignment

										  //! Self-raise Bit properties
		foreach(QVariant var, m_status) {
			setProperty(var.toString().toStdString().c_str(),
				m_monitorBlock.Value(var.toUInt()).toBool());
		}
		//!
		ui.lcdNumberCountOn->display(m_monitorBlock.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_ON_COUNT).toReal());
		ui.lcdNumberCountOff->display(m_monitorBlock.Value(SignalMonitor::OFFSET_MONITOR_WIDTH_OFF_COUNT).toReal());
		break;
	}
	default:
		break;
	}
}

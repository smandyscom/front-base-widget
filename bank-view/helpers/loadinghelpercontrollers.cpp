#include "loadinghelpercontrollers.h"

LoadingHelperControllers::LoadingHelperControllers(QObject *parent) : QObject(parent)
{

}

void LoadingHelperControllers::LoadAdaptors()
{
	m_database = JunctionBankDatabase::Instance();

	m_adaptorList 
		<< new GenericSqlTableAdapter<AxisContextBlock, AxisBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_AXIS))
		<< new GenericSqlTableAdapter<CylinderContext, CylinderBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS))
		<< new GenericSqlTableAdapter<SignalContext, SignalBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_SIGNALS))
		<< new GenericSqlTableAdapter<ExtendedCommandBlock, CommandBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS))
		<< new GenericSqlTableAdapter<UnitConfig, UnitConfigBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_UNITS_CONFIG));
}

void LoadingHelperControllers::LoadTransfer(quint8 clientId, quint16 baseOffset, int interval)
{
    m_controllerTransfer = new ControllerBankTransfer(clientId,baseOffset,interval);
    //! Link
	
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_AXIS,m_adaptorList[0]);
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_CYLINDER, m_adaptorList[1]);
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_SIGNAL, m_adaptorList[2]);
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_COMMAND_BLOCK, m_adaptorList[3]);

    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_UNIT, m_adaptorList[4]);
}

void LoadingHelperControllers::LoadInputsMonitor()
{
    m_controllerInputMonitor = new ControllerIOMonitor(1);

    QMap<QVariant,QVariant> map;
    map[utilities::trimNamespace(QVariant::fromValue(IoAttributes::HAL_ADDRESS))] =
            utilities::trimNamespace(QVariant::fromValue(IoAttributes::NAME));
    m_controllerInputMonitor->setModel(m_database->TableMap(JunctionBankDatabase::WHOLE_INPUTS),
                                       map);
}
void LoadingHelperControllers::LoadOutputsMonitor()
{
    m_controllerOutputMonitor = new ControllerIOMonitor(2);
    QMap<QVariant,QVariant> map;
    map[utilities::trimNamespace(QVariant::fromValue(IoAttributes::HAL_ADDRESS))] =
            utilities::trimNamespace(QVariant::fromValue(IoAttributes::NAME));
    m_controllerOutputMonitor->setModel(m_database->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS),
                                       map);
}
void LoadingHelperControllers::LoadCylinderMonitor()
{
    m_controllerCylinderMonitor = new ControllerIOMonitor(1,ControllerIOMonitor::NO_POLLING);
    QMap<QVariant,QVariant> map{
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_A_1_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_A_1_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_A_2_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_A_2_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_B_1_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_B_1_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_B_2_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::ACT_B_2_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_1_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_1_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_2_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_2_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_3_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_3_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_4_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_A_4_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_1_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_1_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_2_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_2_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_3_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_3_N))},
        {utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_4_H)),utilities::trimNamespace(QVariant::fromValue(CylinderBlock::SEN_B_4_N))},
    };

    m_controllerCylinderMonitor->setModel(m_database->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS),
                                       map);
}

void LoadingHelperControllers::LoadMaterialTransfer()
{
	QSqlTableModel* model = 
		m_database->TableMap(JunctionBankDatabase::DEF_REGION);

	//! filter out Role is not zeros
	model->setFilter(QString("%1<>%2")
		.arg(QVariant::fromValue(ControllerMaterialTransfer::ROLE).toString())
		.arg(QVariant::fromValue(ControllerMaterialTransfer::ROLE_NONE).toInt()));
	//model->select();
	model->sort(0,Qt::AscendingOrder);

	size_t count = model->rowCount();

	for (size_t i = 0; i < model->rowCount(); i++)
	{
		int clientId = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::CLIENT_ID).toString()).toInt();
		int baseOffset = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::BASE_OFFSET).toString()).toInt();
		int interval = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::INTERVAL).toString()).toInt();
		int slotId = model->record(i).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
		ControllerMaterialTransfer::SyncRole role = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::ROLE).toString()).value<ControllerMaterialTransfer::SyncRole>();

		ControllerMaterialTransfer* ref =
			new ControllerMaterialTransfer(clientId, baseOffset, interval);

		AbstractSqlTableAdpater* adaptor =
			new GenericSqlTableAdapter<AbstractDataBlock, SlotBlock::DataBaseHeaders>(JunctionMaterialDatabase::Instance()->TableMap(slotId,JunctionMaterialDatabase::MAT_DATA_SLOT));

		ref->Setup(role, slotId, adaptor);
		m_controllersMaterial.append(ref);
	}
}


void LoadingHelperControllers::ControllersLoadingRoutineV1()
{
    //!
    //! Convertor initialize
    AbstractDataBlock::Dimension = new QMap<DEF_BASIC_DIMENSION::Keys,qreal>();
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::LENGTH] = 0.001;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TIME] = 0.001;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TORQUE_RATIO] = 0.001;
    //!TODO Follow Database to initiate interface channel
    m_database = JunctionBankDatabase::Instance();
    //!TODO Follow Database to decide offset/client
    LoadTransfer(0,512,1);  
    m_controllerMain = new ControllerMainPanel(0,128,200);
    //!
    LoadInputsMonitor();
    LoadOutputsMonitor();
    LoadCylinderMonitor();
	//!Load slot material controller
	//LoadMaterialTransfer();

	emit controllerLoaded();
}

//void LoadingHelperControllers::CrossLink(ControllerBase *controller, FrontCommon *front)
//{
//    controller->AttachReceiver(front);
//    front->LinkController(controller);
//}

void LoadingHelperControllers::CrossLink(QObject* port1, QObject* port2)
{
	PropertyPortCommon* m_port1 = qobject_cast<PropertyPortCommon*>(port1);
	PropertyPortCommon* m_port2 = qobject_cast<PropertyPortCommon*>(port2);

	connect(m_port1, &PropertyPortCommon::externalPropertyChange, m_port2, &PropertyPortCommon::onPropertyChanged,Qt::QueuedConnection);
	connect(m_port2, &PropertyPortCommon::externalPropertyChange, m_port1, &PropertyPortCommon::onPropertyChanged, Qt::QueuedConnection);

}


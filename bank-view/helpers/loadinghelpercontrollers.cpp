#include "loadinghelpercontrollers.h"

LoadingHelperControllers::LoadingHelperControllers(QObject *parent) : QObject(parent)
{

}

void LoadingHelperControllers::LoadTransfer(quint8 clientId, quint16 baseOffset, int interval)
{
    m_controllerTransfer = new ControllerBankTransfer(clientId,baseOffset,interval,qApp);
    //! Link
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_AXIS,new GenericSqlTableAdapter<AxisContextBlock,AxisBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_AXIS)));
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_CYLINDER,new GenericSqlTableAdapter<CylinderContext,CylinderBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS)));
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_SIGNAL,new GenericSqlTableAdapter<SignalContext,SignalBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_SIGNALS)));
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_COMMAND_BLOCK,new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS)));
    m_controllerTransfer->Adaptor(ManualModeDataBlock::SELECTION_UNIT,new GenericSqlTableAdapter<UnitContextBlock,UnitBlock::DataBaseHeaders>(m_database->TableMap(JunctionBankDatabase::WHOLE_UNITS)));
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
	model->select();
	model->sort(0,Qt::AscendingOrder);

	for (size_t i = 0; i < model->rowCount(); i++)
	{
		int clientId = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::CLIENT_ID).toString()).toInt();
		int baseOffset = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::BASE_OFFSET).toString()).toInt();
		int interval = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::INTERVAL).toString()).toInt();
		int slotId = model->record(i).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
		ControllerMaterialTransfer::SyncRole role = model->record(i).value(QVariant::fromValue(ControllerMaterialTransfer::ROLE).toString()).value<ControllerMaterialTransfer::SyncRole>();

		ControllerMaterialTransfer* ref =
			new ControllerMaterialTransfer(clientId, baseOffset, interval,qApp);

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
    LoadTransfer(0,512,100);  
    m_controllerMain = new ControllerMainPanel(0,128,100,qApp);
    //!
    LoadInputsMonitor();
    LoadOutputsMonitor();
    LoadCylinderMonitor();
	//!Load slot material controller
	LoadMaterialTransfer();
}

void LoadingHelperControllers::CrossLink(ControllerBase *controller, FrontCommon *front)
{
    controller->AttachReceiver(front);
    front->LinkController(controller);
}

JunctionBankDatabase* LoadingHelperControllers::m_database = nullptr;
ControllerBankTransfer* LoadingHelperControllers::m_controllerTransfer = nullptr;
ControllerMainPanel* LoadingHelperControllers::m_controllerMain = nullptr;
ControllerIOMonitor* LoadingHelperControllers::m_controllerInputMonitor = nullptr;
ControllerIOMonitor* LoadingHelperControllers::m_controllerOutputMonitor = nullptr;
ControllerIOMonitor* LoadingHelperControllers::m_controllerCylinderMonitor = nullptr;
QList<ControllerMaterialTransfer*> LoadingHelperControllers::m_controllersMaterial;
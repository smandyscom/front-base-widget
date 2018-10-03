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

void LoadingHelperControllers::LoadInputsMonitor(int interval)
{
    m_controllerInputMonitor = new ControllerIOMonitor(interval,qApp);
    m_controllerInputMonitor->AppendPair(QVariant::fromValue(IoAttributes::HAL_ADDRESS),
                                         QVariant::fromValue(IoAttributes::NAME));
    m_controllerInputMonitor->AttachReceiver(m_database->TableMap(JunctionBankDatabase::WHOLE_INPUTS));
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
    LoadInputsMonitor(100);
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

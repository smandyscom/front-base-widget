#include "loadinghelper.h"

LoadingHelper::LoadingHelper(QObject *parent) : QObject(parent)
{

}

void LoadingHelper::LoadBaseDatabase()
{
}

void LoadingHelper::LoadFrontAxis(FrontAxisParameter* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_REGION),
                  m_instance->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_COMMAND_BLOCKS),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_AXIS));
	widget->activateBlink();
}
void LoadingHelper::LoadFrontCylinder(FrontCylinderPanel* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_REGION),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_CYLINDERS));
}
void LoadingHelper::LoadFrontUnit(FrontUnitPanel* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_UNITS),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_UNIT),
				  m_instance->TableMap(JunctionBankDatabase::WHOLE_UNITS_CONFIG),
				m_instance->TableMap(JunctionBankDatabase::HEADER_UNIT_CONFIG));
}

void LoadingHelper::LoadFrontConfiguration(FrontConfigurationTransfer* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_UNITS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_SIGNALS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE));
}

void LoadingHelper::LoadFrontIO(FrontIoOverride *widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_INPUTS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_REGION),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_IO));
}

void LoadingHelper::LoadSafeIO(FrontSafetyPanel * widget)
{
	widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_SAFETY_INPUTS),
		m_instance->TableMap(JunctionBankDatabase::WHOLE_SAFETY_OUTPUTS),
		m_instance->TableMap(JunctionBankDatabase::HEADER_IO));
}



void LoadingHelper::LoadFrontMain(FrontMainPanel* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_UNITS),
                  m_instance->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::ERROR_CODE_CYLINDER),
                  m_instance->TableMap(JunctionBankDatabase::ERROR_CODE_UNIT),
                  m_instance->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE),
				  m_instance->TableMap(JunctionBankDatabase::CONTROLLER_MONITOR),
				  m_instance->TableMap(JunctionBankDatabase::HEADER_MONITOR));
	widget->activateBlink();
}

void LoadingHelper::CombineModelViewV1(FrontAxisParameter* widget1,
                                       FrontCylinderPanel* widget2,
                                       FrontUnitPanel* widget3,
                                       FrontConfigurationTransfer* widget4,
                                       FrontMainPanel* widget5,
	FrontIoOverride *widget6,
	FrontSafetyPanel* widget7,
	FrontSignal* widget8)
{

    m_instance = JunctionBankDatabase::Instance();

    LoadFrontAxis(widget1);
    LoadFrontCylinder(widget2);
    LoadFrontUnit(widget3);
    //LoadFrontConfiguration(widget4);
    LoadFrontMain(widget5);
    LoadFrontIO(widget6);
	LoadSafeIO(widget7);

	widget8->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_SIGNALS),
		m_instance->TableMap(JunctionBankDatabase::HEADER_SIGNALS));
}



JunctionBankDatabase* LoadingHelper::m_instance = nullptr;

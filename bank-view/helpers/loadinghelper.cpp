#include "loadinghelper.h"

LoadingHelper::LoadingHelper(QObject *parent) : QObject(parent)
{

}

void LoadingHelper::LoadFrontAxis(FrontAxisParameter* widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_REGION),
                  m_instance->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS),
                  m_instance->TableMap(JunctionBankDatabase::HEADER_COMMAND_BLOCKS));
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
                  m_instance->TableMap(JunctionBankDatabase::HEADER_UNIT));
}

void LoadingHelper::LoadFrontConfiguration(FrontConfigurationTransfer* widget)
{

}

void LoadingHelper::LoadFrontIO(FrontIoOverride *widget)
{
    widget->Setup(m_instance->TableMap(JunctionBankDatabase::WHOLE_INPUTS),
                  m_instance->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS),
                  m_instance->TableMap(JunctionBankDatabase::DEF_REGION),
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
                  m_instance->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE));
}

void LoadingHelper::CombineModelViewV1(FrontAxisParameter* widget1,
                                       FrontCylinderPanel* widget2,
                                       FrontUnitPanel* widget3,
                                       FrontConfigurationTransfer* widget4,
                                       FrontMainPanel* widget5, FrontIoOverride *widget6)
{

    m_instance = JunctionBankDatabase::Instance();
    m_instance->onInitialize();

    LoadFrontAxis(widget1);
    LoadFrontCylinder(widget2);
    LoadFrontUnit(widget3);
    LoadFrontConfiguration(widget4);
    LoadFrontMain(widget5);
    LoadFrontIO(widget6);

    m_instance->onReleaseHeaders();
}

void LoadingHelper::CrossLink(ControllerBase *controller, FrontCommon *front)
{
    controller->AttachReceiver(front);
    front->LinkController(controller);
}

JunctionBankDatabase* LoadingHelper::m_instance = nullptr;
#include "frontcommonmanual.h"

FrontCommonManual::FrontCommonManual(QWidget *parent):
    FrontCommon (parent)
{

}


void FrontCommonManual::showEvent(QShowEvent* event)
{
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str,
                              m_categrory);
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str,
                              m_index);
    //! Base method
    FrontCommon::showEvent(event);
}

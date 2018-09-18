#include "frontcommonmanual.h"

FrontCommonManual::FrontCommonManual(QWidget *parent):
    FrontCommon (parent),
    mainDataTable(nullptr)
{

}

int FrontCommonManual::currentIndex() const
{
    return 0;
}
QString FrontCommonManual::currentFilter() const
{
    return nullptr;
}

void FrontCommonManual::onMonitorIndexChanged()
{
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str(),
                              currentIndex());
}

void FrontCommonManual::showEvent(QShowEvent* event)
{
    //! Monitor selection
    if(m_controller!=nullptr)
    {
        m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                                  m_categrory);
        onMonitorIndexChanged();
    }
    if(mainDataTable!=nullptr)
    {
        //! Resume
        mainDataTable->setFilter(currentFilter());
        mainDataTable->select();
    }
    //! Base method
    FrontCommon::showEvent(event);
}

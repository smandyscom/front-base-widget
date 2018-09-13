#include "frontmainpanel.h"
#include "ui_frontmainpanel.h"

FrontMainPanel::FrontMainPanel(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontMainPanel),
    locale(zh_TW)
{
    ui->setupUi(this);

    //!
      m_errorDeviceMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
      m_errorDeviceMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS);
      m_errorDeviceMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS);

      m_errorCodeMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS);
      m_errorCodeMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_CYLINDER);
      m_errorCodeMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_UNIT);

      m_deviceTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);
}

FrontMainPanel::~FrontMainPanel()
{
    delete ui;
}
//!
//! \brief FrontMainPanel::dynamicPropertyChanged
//! \param key
//! Post Strings or rework properties
void FrontMainPanel::dynamicPropertyChanged(int key)
{
    switch (key) {
    case MainMonitorBlock::ERROR_CODE:
        ui->textBrowserErrorDescription->setText(errorDescription(
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_CATEGRORY).toString().toStdString().c_str()).toInt(),
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_DEVICE_INDEX).toString().toStdString().c_str()).toInt(),
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_CODE).toString().toStdString().c_str()).toInt()
                                                     ));
        break;
    case MainOperationBlock::OFFSET_UOB_STATE_PAUSE:
    case MainOperationBlock::BIT_3_TOGGLE_MANUAL:
        setProperty("MainState",QVariant::fromValue(mainState(
                                                        property(QVariant::fromValue(MainOperationBlock::OFFSET_UOB_STATE_PAUSE).toString().toStdString().c_str()).toBool(),
                                                        property(QVariant::fromValue(MainOperationBlock::BIT_3_TOGGLE_MANUAL).toString().toStdString().c_str()).toBool()
                                                        )));
        break;
    default:
        break;
    }
}

FrontMainPanel::MainStates FrontMainPanel::mainState(bool isPause, bool isManual)
{
    if(!isPause)
        return STATE_AUTO;
    else if(isManual)
        return STATE_MANUAL;
    else
        return STATE_SEMI_AUTO;
}
QString FrontMainPanel::errorDescription(int deviceCategrory,int deviceIndex,int errorCode)
{
    QSqlRecord recordIndex;

    if(m_errorDeviceMap.contains(deviceCategrory))
        recordIndex = m_errorDeviceMap[deviceCategrory]->record(deviceIndex);

    QSqlRecord recordDevice =
            utilities::getSqlTableSelectedRecord(m_deviceTable,
                                                 QVariant::fromValue(ID),
                                                 QVariant::fromValue(deviceCategrory));
    if(errorCode==0)
        return QString("");

    QSqlTableModel* __lookupTable = m_errorCodeMap[deviceCategrory];

    if(__lookupTable == nullptr)
        return QString("%1")
                .arg("no description");

    QString __description;
    for(int i=0;i<__lookupTable->rowCount();i++)
    {
        QSqlRecord __record = __lookupTable->record(i);
        MODBUS_U_QUAD __ref = __record.value(QVariant::fromValue(ID).toString()).toULongLong();
        if((__ref & errorCode)!=0)
            __description.append(QString("%1\n").arg(__record.value(QVariant::fromValue(locale).toString()).toString()));
    }

    return QString("%1,%2,%3")
            .arg(recordDevice.value(QVariant::fromValue(locale).toString()).toString())
            .arg(recordIndex.value(QVariant::fromValue(NAME).toString()).toString())
            .arg(recordIndex.value(QVariant::fromValue(locale).toString()).toString()) +
            QString("%1")
            .arg(__description);
}

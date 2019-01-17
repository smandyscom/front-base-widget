#include "frontmainpanel.h"
#include "ui_frontmainpanel.h"

FrontMainPanel::FrontMainPanel(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontMainPanel),
    locale(zh_TW)
{
    ui->setupUi(this);
    //! Bind
    ui->pushButtonPause->setProperty(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str(),
                                     QVariant::fromValue(MainOperationBlock::BIT_1_TOGGLE_PAUSE));
    ui->pushButtonInitialize->setProperty(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str(),
                                     QVariant::fromValue(MainOperationBlock::BIT_2_TOGGLE_INIT));
    ui->pushButtonClear->setProperty(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str(),
                                     QVariant::fromValue(MainOperationBlock::BIT_5_TOGGLE_CLEAR));
    ui->pushButtonErrorReset->setProperty(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str(),
                                     QVariant::fromValue(MainOperationBlock::BIT_0_ERROR_RESET));
    ui->pushButtonErrorIgnore->setProperty(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str(),
                                     QVariant::fromValue(MainOperationBlock::BIT_4_ERROR_IGNORE));
    foreach (QPushButton* var, findChildren<QPushButton*>()){
        connect(var,&QPushButton::clicked,this,&FrontMainPanel::onButtonClicked);
        m_widgetsPolish.append(var);
    }
	m_widgetsPolish.append(ui->textBrowserErrorDescription);

	//!
	m_delayTimer = new QTimer(this);
	m_delayTimer->setInterval(500);
	connect(m_delayTimer, &QTimer::timeout, this, &FrontMainPanel::onDelayTimeout);
}

FrontMainPanel::~FrontMainPanel()
{
    delete ui;
}

void FrontMainPanel::onButtonClicked()
{
	QVariant value = sender()->property(QVariant::fromValue(RELATED_BIT).toString().toStdString().c_str());

	emit qobject_cast<PropertyPortCommon*>(m_port)->externalPropertyChange(value,
		true);
}


void FrontMainPanel::Setup(QSqlTableModel* axisTable,
                           QSqlTableModel* cylinderTable,
                           QSqlTableModel* unitTable,
                           QSqlTableModel* axisErrorTable,
                           QSqlTableModel* cylinderErrorTable,
                           QSqlTableModel* unitErrorTable,
                           QSqlTableModel* typeTable,
						QSqlTableModel* monitor,
						QSqlTableModel* monitorHeader)
{
    //!
      m_errorDeviceMap[ManualModeDataBlock::SELECTION_AXIS] = axisTable;
      m_errorDeviceMap[ManualModeDataBlock::SELECTION_CYLINDER] = cylinderTable;
      m_errorDeviceMap[ManualModeDataBlock::SELECTION_UNIT] = unitTable;

      m_errorCodeMap[ManualModeDataBlock::SELECTION_AXIS] = axisErrorTable;
      m_errorCodeMap[ManualModeDataBlock::SELECTION_CYLINDER] = cylinderErrorTable;
      m_errorCodeMap[ManualModeDataBlock::SELECTION_UNIT] = unitErrorTable;
	  //!
	  ui->tableViewMonitor->setModel(monitor);
	  HEADER_STRUCTURE::HeaderRender::renderViewHeader(monitorHeader,
		  ui->tableViewMonitor);
	  m_monitorTable = monitor;

      m_deviceTable = typeTable;
}

//!
//! \brief FrontMainPanel::dynamicPropertyChanged
//! \param key
//! Post Strings or rework properties
void FrontMainPanel::dynamicPropertyChanged(int key, QVariant value)
{
    switch (key) {
    case MainMonitorBlock::ERROR_CODE:
		if (value.toInt() == 0)
		{
			ui->textBrowserErrorDescription->clear();
			setProperty(QVariant::fromValue(ERROR_STATE).toString().toStdString().c_str(),false);
		}
		else
		{
			ui->textBrowserErrorDescription->setText(errorDescription(
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_CATEGRORY).toString().toStdString().c_str()).toInt(),
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_DEVICE_INDEX).toString().toStdString().c_str()).toInt(),
                                                     property(QVariant::fromValue(MainMonitorBlock::ERROR_CODE).toString().toStdString().c_str()).toInt()
                                                     ));
			setProperty(QVariant::fromValue(ERROR_STATE).toString().toStdString().c_str(), true);
		}//else     
        break;
    case MainOperationBlock::OFFSET_UOB_STATE_PAUSE:
    case MainOperationBlock::BIT_1_TOGGLE_PAUSE:
    case MainOperationBlock::BIT_3_TOGGLE_MANUAL:
        m_state = mainState(
                    property(QVariant::fromValue(MainOperationBlock::OFFSET_UOB_STATE_PAUSE).toString().toStdString().c_str()).toBool(),
                    property(QVariant::fromValue(MainOperationBlock::BIT_3_TOGGLE_MANUAL).toString().toStdString().c_str()).toBool()
                    );
        setProperty(QVariant::fromValue(MAINSTATE).toString().toStdString().c_str(),
                    QVariant::fromValue(m_state));
        break;
	case MainOperationBlock::MON_DATA_0:
	case MainOperationBlock::MON_DATA_1:
	case MainOperationBlock::MON_DATA_2:
	case MainOperationBlock::MON_DATA_3:
	case MainOperationBlock::MON_DATA_4:
	case MainOperationBlock::MON_DATA_5:
	case MainOperationBlock::MON_DATA_6:
	case MainOperationBlock::MON_DATA_7:
	{
		m_monitorTable->select(); //reload

		int index = (key - MainOperationBlock::MON_DATA_0)/2;
		QSqlRecord record = m_monitorTable->record(index);
		qreal p_value = record.value(QVariant::fromValue(MainBlock::FACTOR).toString()).toReal() * value.toReal();
		record.setValue(QVariant::fromValue(MainBlock::VALUE).toString(), p_value);
		p_value = record.value(QVariant::fromValue(MainBlock::VALUE).toString()).toReal();

		bool result = m_monitorTable->setRecord(index, record);
		
		break;
	}
	case ManualModeDataBlock::PROP_MANUAL_STATE:
		switch (value.value<ManualModeDataBlock::ManualState>())
		{
		//case ManualModeDataBlock::STATE_PLC_READY:	
		//case ManualModeDataBlock::STATE_IN_AUTO:
		//	//setEnabled(true);
		//	break;
		case ManualModeDataBlock::STATE_RUN_ON:
		case ManualModeDataBlock::STATE_DONE_ON:
			setEnabled(false);
			m_delayTimer->start(); //restart
			break;
		default:
			break;
		}
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
    
    if(errorCode==0)
        return QString(""); //clear screen

	if (!m_errorDeviceMap.contains(deviceCategrory))
		return QString("%4[%1:%2:%3]")
		.arg(deviceCategrory)
		.arg(deviceIndex)
		.arg(errorCode)
		.arg("no description");

	QSqlRecord recordIndex = utilities::getSqlTableSelectedRecord(m_errorDeviceMap[deviceCategrory],
			QVariant::fromValue(ID),
			QVariant::fromValue(deviceIndex));

    QSqlRecord recordDevice =
            utilities::getSqlTableSelectedRecord(m_deviceTable,
                                                 QVariant::fromValue(ID),
                                                 QVariant::fromValue(deviceCategrory));


   
	//represented by unique error id
	QSqlRecord recordDescription = utilities::getSqlTableSelectedRecord(m_errorDeviceMap[deviceCategrory],
		QVariant::fromValue(ID),
		QVariant::fromValue(errorCode));
		
	QString device = recordDevice.isEmpty() ?
		QString(deviceCategrory) :
		recordDevice.value(QVariant::fromValue(locale).toString()).toString();

	qDebug() << recordIndex.value(QVariant::fromValue(NAME).toString()).isNull();
	QString index = recordIndex.value(QVariant::fromValue(NAME).toString()).isNull() ?
		QString::number(deviceIndex) :
		recordIndex.value(QVariant::fromValue(NAME).toString()).toString() + 
		recordIndex.value(QVariant::fromValue(locale).toString()).toString();

	qDebug() << recordDescription.value(QVariant::fromValue(locale).toString()).isNull();
	QString description = recordDescription.value(QVariant::fromValue(locale).toString()).isNull() ?
		QString("0x%1").arg(QString::number(errorCode,16)) :
		recordDescription.value(QVariant::fromValue(locale).toString()).toString();

	//represented by bit occurance (Stupid MIII representation
    /*for(int i=0;i<lookup->rowCount();i++)
    {
        QSqlRecord record = lookup->record(i);
        MODBUS_U_QUAD ref = record.value(QVariant::fromValue(ID).toString()).toULongLong();
        if((ref & errorCode)!=0)
            description.append(QString("%1\n").arg(record.value(QVariant::fromValue(locale).toString()).toString()));
    }*/

	return QString("%1,%2,%3")
		.arg(device)
		.arg(index)
		.arg(description);
}

void FrontMainPanel::onDelayTimeout()
{
	switch (property(QVariant::fromValue(ManualModeDataBlock::PROP_MANUAL_STATE).toString().toStdString().c_str()).value<ManualModeDataBlock::ManualState>())
	{
	case ManualModeDataBlock::STATE_PLC_READY:
	case ManualModeDataBlock::STATE_IN_AUTO:
		setEnabled(true);
		break;
	default:
		break;
	}
}
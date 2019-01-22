
#include "controllerbanktransfer.h"

bool operator==(const TransferTask& lhp, const TransferTask&rhp)
{
	return lhp.command == rhp.command &&
		lhp.categrory == rhp.categrory &&
		lhp.deviceIndex == rhp.deviceIndex;
}


ControllerBankTransfer::ControllerBankTransfer(quint8 clientId, quint16 baseOffset, int interval=100, QObject *parent) :
    ControllerManualMode(clientId,baseOffset,interval,parent)
{
}

void ControllerBankTransfer::Adaptor(ManualModeDataBlock::Categrories key,AbstractSqlTableAdpater* value)
{
    m_adaptors[key] = value;
    //! Sense data changed and put task
    connect(value->Model(),&QSqlTableModel::dataChanged,this,&ControllerBankTransfer::onDataChanged);
	//! replace with duplicated table
	value->Model(utilities::duplicate(value->Model()));
}

//!
//! \brief ControllerBankTransfer::plcReady
//! Iteration begin
void ControllerBankTransfer::plcReady()
{
	if (!m_tasksQueue.isEmpty())
	{
		//! Auto started
		transfer();
	}
	else
	{
		switch (m_mode())
		{
			case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
				emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::PROP_MODEL_UPDATED),
					true);
		default:
			break;
		}
	}

    //!Base method
    ControllerManualMode::plcReady();
}
//!
//! \brief ControllerBankTransfer::onOperationTrigger
//! After RUN
void ControllerBankTransfer::doneOn()
{   
    switch (m_mode()) {
    //! Once triggered as Transfer mode
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
        m_channel->BeginRead(toAddressMode(ManualModeDataBlock::DATA_BLOCK_HEAD),QVariant::fromValue(CellDataBlock()));
        break;
    default:
        break;
    }

    //!
    //! \brief ControllerManualMode::doneOn
    //! Run off
    ControllerManualMode::doneOn();
}
//!
//! \brief ControllerBankTransfer::doneOff
//! Once task queue not empty ,
//! Re-initiate Run
void ControllerBankTransfer::doneOff()
{
    switch (m_mode()) {
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
    {
        //! Write back to Database
        //! synced with BeginRead since Run off came after run
        CellDataBlock data =
                qobject_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::DATA_BLOCK_HEAD).value<CellDataBlock>();
        m_adaptors[m_categrory]->Record(m_index,
			AbstractDataBlock(reinterpret_cast<MODBUS_U_WORD*>(&data)),
			AbstractSqlTableAdpater::KEY_NAMED_KEY,
			QVariant::fromValue(HEADER_STRUCTURE::ID));
    }
    case ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK:
		if (m_tasksQueue.isEmpty())
			qDebug() << "Bank transfer error with empty queue";
		else
			m_tasksQueue.dequeue(); //take one out
        break;
    default:
        break;
    }
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::PROP_QUEUE_REMAIN),
		m_tasksQueue.count());

    ControllerManualMode::doneOff();
}


void ControllerBankTransfer::onDataChanged(const QModelIndex &topLeft,
                                           const QModelIndex &bottomRight,
                                           const QVector<int> &roles)
{
    if(!roles.isEmpty())
        if ((roles.first() == HEADER_STRUCTURE::UserRole_MonitorOnOff ||
            roles.first() == HEADER_STRUCTURE::UserRole_OverrideOnOff))
            return;

    AbstractSqlTableAdpater* adaptor = nullptr;
	QSqlTableModel* model = qobject_cast<QSqlTableModel*>(sender());
    //! Search for Adaptor
    foreach (AbstractSqlTableAdpater* var, m_adaptors.values()) {
        if(var->Model()->tableName()==
			model->tableName())
        {
            adaptor = var;
            break;
        }
    }
    //!
	//! Turns into absolute ID
	TransferTask task{ ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE,
		m_adaptors.key(adaptor) ,
		model->record(topLeft.row()).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt()};

    if(!m_tasksQueue.contains(task))
        m_tasksQueue.enqueue(task);
}

//!
//! \brief ControllerBankTransfer::m_operator_propertyChanged
//! \param key
//! \param value
//! Trigger transfer task
void ControllerBankTransfer::onPropertyChanged(QVariant key, QVariant value)
{  
    //! Prepare
    switch (key.toUInt()) {
    case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
    case ManualModeDataBlock::BATCH_ALL_READ_MODE:

		

		//! First time enter mode
        switch (value.value<ManualModeDataBlock::Categrories>()) {
        case ManualModeDataBlock::SELECTION_ALL:
            foreach (AbstractSqlTableAdpater* var, m_adaptors.values()) {

				var->Model()->setFilter(nullptr);
				//var->Model()->select();

				int rowCount = var->Model()->rowCount();

				for (int i = 0; i < rowCount; i++) {
					TransferTask task{ key.value<ManualModeDataBlock::TransferCommand>(),
						m_adaptors.key(var),
						var->Model()->record(i).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt()};
					m_tasksQueue.enqueue(task);
				}
            }
            break;
        case ManualModeDataBlock::SELECTION_COMMAND_BLOCK:
        case ManualModeDataBlock::SELECTION_AXIS:
        case ManualModeDataBlock::SELECTION_CYLINDER:
        case ManualModeDataBlock::SELECTION_UNIT:
        case ManualModeDataBlock::SELECTION_SIGNAL:
        {
            ManualModeDataBlock::Categrories var = value.value<ManualModeDataBlock::Categrories>();
			m_adaptors[var]->Model()->setFilter(nullptr);
			//m_adaptors[var]->Model()->select();
			int count = m_adaptors[var]->Model()->rowCount();
			for (int i = 0; i < m_adaptors[var]->Model()->rowCount(); i++) {
				TransferTask task{ key.value<ManualModeDataBlock::TransferCommand>(),
					var,
					m_adaptors[var]->Model()->record(i).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt()};
				m_tasksQueue.enqueue(task);
			}
            
        }
		break;
		
        default:
            //!No action
            break;
        }//inner switch
		
        break;


    default:
        //!No action
        break;
    }//outer switch

    //!Action
	switch (key.toUInt()) {
	case ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE:
	case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
	case ManualModeDataBlock::BATCH_ALL_READ_MODE:
	{
        //! trigger operation , raise first shot
		if (m_currentState == ManualModeDataBlock::STATE_PLC_READY &&
			!m_tasksQueue.isEmpty())
		{
			emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::PROP_QUEUE_COUNT),
				m_tasksQueue.count());

			transfer();
		}

        break;
	}
    default:
        ControllerManualMode::onPropertyChanged(key,value);
        break;
    }
}

void ControllerBankTransfer::transfer()
{
    m_categrory = m_tasksQueue.head().categrory;
    m_index = m_tasksQueue.head().deviceIndex;

    //! Write anyway
    CellDataBlock* data =
            reinterpret_cast<CellDataBlock*>(m_adaptors[m_categrory]->Record(m_index,
				AbstractSqlTableAdpater::KEY_NAMED_KEY,
				QVariant::fromValue(HEADER_STRUCTURE::ID)).Anchor());
	
	//!
	ManualModeDataBlock::TransferCommand command = m_tasksQueue.head().command;

	ManualModeDataBlock::Mode mode = (command == ManualModeDataBlock::BATCH_ALL_READ_MODE ?
		ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK :
		ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK);

	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE), QVariant::fromValue(mode));

	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD), QVariant::fromValue(*data));
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY), QVariant::fromValue(m_categrory));
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX), QVariant::fromValue(m_index));
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN), true);

	
}

ManualModeDataBlock::Mode ControllerBankTransfer::m_mode()
{
    return qobject_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::COMMIT_MODE).value<ManualModeDataBlock::Mode>();
}

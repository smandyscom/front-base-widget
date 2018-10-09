
#include "controllerbanktransfer.h"

ControllerBankTransfer::ControllerBankTransfer(quint8 clientId, quint16 baseOffset, int interval=100, QObject *parent) :
    ControllerManualMode(clientId,baseOffset,interval,parent)
{
//    connect(this,&ControllerBankTransfer::dataTransfered,[=](){
//       //! cut link after done
//       disconnect(__controller,SIGNAL(operationReady()),this,SLOT(onControllerOperationReady()));
//    });
    //!Operators
    QList<QVariant> m_operator_list = {
        QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE),
        QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_WRITE_MODE),
        QVariant::fromValue(ManualModeDataBlock::BATCH_ALL_READ_MODE)
    };
    foreach (QVariant var, m_operator_list)
    {
        m_operator_propertyKeys[var.toString()] = var;
    }
}

void ControllerBankTransfer::Adaptor(ManualModeDataBlock::Categrories key,AbstractSqlTableAdpater* value)
{
    m_adaptors[key] = value;
    //! Sense data changed and put task
    connect(value->Model(),&QSqlTableModel::dataChanged,this,&ControllerBankTransfer::onDataChanged);
}

//!
//! \brief ControllerBankTransfer::plcReady
//! Iteration begin
void ControllerBankTransfer::plcReady()
{
    switch (m_mode()) {
    case ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK:
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
    {
		if (!m_tasksQueue.isEmpty())
		{
			//! Auto started
			transfer(QVariant::fromValue(m_mode()));
		}
		else
		{
			//setProperty();

		}
        break;
    }
    default:
        break;
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
        m_adaptors[m_categrory]->Record(m_index,AbstractDataBlock(reinterpret_cast<MODBUS_U_WORD*>(&data)));
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
    //! Search for Adaptor
    foreach (AbstractSqlTableAdpater* var, m_adaptors.values()) {
        if(var->Model()==sender())
        {
            adaptor = var;
            break;
        }
    }
    //!
    TransferTask task;
    task.first = m_adaptors.key(adaptor) ;
    //! Turns into absolute row index
    task.second =  adaptor->Model()->record( topLeft.row())
             .value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString())
             .toInt();

    if(!m_tasksQueue.contains(task))
        m_tasksQueue.enqueue(task);
}

//!
//! \brief ControllerBankTransfer::m_operator_propertyChanged
//! \param key
//! \param value
//! Trigger transfer task
void ControllerBankTransfer::m_operator_propertyChanged(QVariant key, QVariant value)
{  
    //! Prepare
    switch (key.toUInt()) {
    case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
    case ManualModeDataBlock::BATCH_ALL_READ_MODE:
		if (!value.toBool())
			return;
		//! First time enter mode
        switch (value.value<ManualModeDataBlock::Categrories>()) {
        case ManualModeDataBlock::SELECTION_ALL:
            foreach (AbstractSqlTableAdpater* var, m_adaptors.values()) {
                for(int i=0;i<var->Model()->rowCount();i++)
                    m_tasksQueue.enqueue(TransferTask(m_adaptors.key(var),i));
            }
            break;
        case ManualModeDataBlock::SELECTION_COMMAND_BLOCK:
        case ManualModeDataBlock::SELECTION_AXIS:
        case ManualModeDataBlock::SELECTION_CYLINDER:
        case ManualModeDataBlock::SELECTION_UNIT:
        case ManualModeDataBlock::SELECTION_SIGNAL:
        {
            ManualModeDataBlock::Categrories var = value.value<ManualModeDataBlock::Categrories>();
            AbstractSqlTableAdpater* adaptor = m_adaptors[var];
            for(int i=0;i<m_adaptors[var]->Model()->rowCount();i++)
                m_tasksQueue.enqueue(TransferTask(var,i));
            break;
        }
        default:
            //!No action
            break;
        }
        break;
    default:
        //!No action
        break;
    }

    
    //!Action
    switch (key.toUInt()) {
    case ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE:
    case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
    case ManualModeDataBlock::BATCH_ALL_READ_MODE:
        //! False , no action
        if(!value.toBool())
            return;

        //! trigger operation , raise first shot
        if(m_currentState == ManualState::STATE_PLC_READY &&
                !m_tasksQueue.isEmpty())
            transfer(key);

        //setProperty(key.toString().toStdString().c_str(),false); //reset property
        break;
    default:
        ControllerManualMode::m_operator_propertyChanged(key,value);
        break;
    }
}

void ControllerBankTransfer::transfer(QVariant mode)
{
    m_categrory = m_tasksQueue.head().first;
    m_index = m_tasksQueue.head().second;

	//! Write mode
	switch (mode.toUInt()) {

	case ManualModeDataBlock::BATCH_ALL_WRITE_MODE:
	case ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE:
		//! Clear task queue (Write
		setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
			QVariant::fromValue(ManualModeDataBlock::MODE_DOWNLOAD_DATA_BLOCK));
		break;
	case ManualModeDataBlock::BATCH_ALL_READ_MODE:
		setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
			QVariant::fromValue(ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK));
		break;
	default:
		break;
	}

    //! Write anyway
    CellDataBlock* data =
            reinterpret_cast<CellDataBlock*>(m_adaptors[m_categrory]->Record(m_index).Anchor());
	setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
		QVariant::fromValue(*data));
	setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
		QVariant::fromValue(m_categrory));
	setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str(),
		QVariant::fromValue(m_index));
	setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),true);
}

ManualModeDataBlock::Mode ControllerBankTransfer::m_mode()
{
    return qobject_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::COMMIT_MODE).value<ManualModeDataBlock::Mode>();
}

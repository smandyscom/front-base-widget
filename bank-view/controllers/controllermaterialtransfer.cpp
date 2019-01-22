#include "controllermaterialtransfer.h"
#include <QDebug>




ControllerMaterialTransfer::ControllerMaterialTransfer(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
	ControllerBase(clientId, baseOffset, interval, parent),
	m_lastActStatus(false)
{
	//! Monitor all 128Words
	m_monitor = new SlotDataBlock(registerWatchList(SlotDataBlock::WORD_OUT, QVariant::fromValue(DataBlock128())));
    //!
	QVariant act = QVariant::fromValue(SlotDataBlock::BIT1_ACT);
	QVariant valid = QVariant::fromValue(SlotDataBlock::BIT2_VALID);
	m_monitor_propertyKeys << act;
	m_monitor_propertyKeys << valid;
	m_monitor_propertyKeys << QVariant::fromValue(SlotDataBlock::MATERIAL_ID);
}

ControllerMaterialTransfer::~ControllerMaterialTransfer()
{

}

//!Select different handling routine
void ControllerMaterialTransfer::Setup(SlotDataBlock::SyncRole role, int index, AbstractSqlTableAdpater* adaptor)
{
	//! Role
	switch (role)
	{
	case SlotDataBlock::ROLE_UPDATE_HEADER:
		break;
	case SlotDataBlock::ROLE_UPDATE_BLOCK:
		connect(this, &ControllerMaterialTransfer::actionRaised, this, &ControllerMaterialTransfer::onTableUpdate);
		break;
	case SlotDataBlock::ROLE_CREATE:
		connect(this, &ControllerMaterialTransfer::actionRaised, this, &ControllerMaterialTransfer::onTableInsert);
		break;
	case SlotDataBlock::ROLE_QUERY:
	case SlotDataBlock::ROLE_QUERY_PASSIVE:
		connect(this, &ControllerMaterialTransfer::actionRaised, this, &ControllerMaterialTransfer::onTableQuery);
		break;
	default:
		break;
	}

	m_adpator = adaptor;
	m_slotIndex = index;
	m_role = role;
	//! report slot index to receiver?
}

int ControllerMaterialTransfer::Index() const
{
	return m_slotIndex;
}

void ControllerMaterialTransfer::onAcknowledged(InterfaceRequest ack)
{
	//!Once Act
	if ( (m_monitor->Value(SlotDataBlock::BIT1_ACT).toBool() && m_role != SlotDataBlock::ROLE_QUERY_PASSIVE) 
		|| (!m_monitor->Value(SlotDataBlock::BIT1_ACT).toBool() && m_lastActStatus && m_role==SlotDataBlock::ROLE_QUERY_PASSIVE))
	{
		m_materialId = m_monitor->Value(SlotDataBlock::MATERIAL_ID).value<MODBUS_U_LONG>();
		m_isValid = m_monitor->Value(SlotDataBlock::BIT2_VALID).toBool();

		emit actionRaised(); //re-direct to specific function
		emit m_port->externalPropertyChange(QVariant::fromValue(m_role), m_materialId);

		if(m_monitor->Value(SlotDataBlock::BIT1_ACT).toBool())
			onPropertyChanged(QVariant::fromValue(SlotDataBlock::BIT1_ACT), false);
	}
	m_lastActStatus = m_monitor->Value(SlotDataBlock::BIT1_ACT).toBool();

	//! After database updated
	ControllerBase::onAcknowledged(ack); //inform front to update
}

void ControllerMaterialTransfer::onTableInsert()
{
    QElapsedTimer stopWatch;
    stopWatch.start();

    //!insert record on table
	QSqlTableModel* m_table = m_adpator->Model();
    QSqlRecord record = m_table->record();

    for(int i=0;i<record.count();i++)
    {
        record.setGenerated(i,true); //all fields would be autogenerated
        record.value(i) = QVariant::fromValue(nullptr);
    }

    //! insert into last row
    if(!m_table->insertRecord(-1,record))
    {
        qDebug() << QString("Insert record error , %1").arg(m_slotIndex);
        qDebug() << m_table->tableName();
        qDebug() << m_table->lastError().text();
    }

    //! Carries largest id
    m_table->setFilter(QString("%2 = (SELECT MAX(%2)  FROM %1)")
                       .arg(m_table->tableName())
                       .arg(QVariant::fromValue(SlotBlock::ID).toString()));
    //m_table->select();//re-select

    record = m_table->record(0); //access last row
    m_materialId = record.value(QVariant::fromValue(SlotBlock::ID).toString()).toInt();

	
	
	//!Write-into bus
	onPropertyChanged(QVariant::fromValue(SlotDataBlock::MATERIAL_ID), m_materialId);

    qDebug() << QString("%1,onInsert elapsed,%2,%3").arg(m_slotIndex).arg(stopWatch.elapsed()).arg(m_materialId);

}
void ControllerMaterialTransfer::onTableQuery()
{
    QElapsedTimer stopWatch;
    stopWatch.start();

    AbstractDataBlock m_adb(m_adpator->Record(m_materialId,
		AbstractSqlTableAdpater::KEY_NAMED_KEY,
		QVariant::fromValue(SlotBlock::ID)).Anchor());

	//! Write-into Bus
	if(m_role != SlotDataBlock::ROLE_QUERY_PASSIVE)
		onPropertyChanged(QVariant::fromValue(SlotDataBlock::BLOCK_DATA),
			QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(m_adb.Anchor())));

    qDebug() << QString("%1,onQuery elapsed,%2,%3").arg(m_slotIndex).arg(stopWatch.elapsed()).arg(m_materialId);
}
void ControllerMaterialTransfer::onTableUpdate()
{
    QElapsedTimer stopWatch;
    stopWatch.start();

	CellDataBlock data = m_monitor->Value(SlotDataBlock::BLOCK_DATA).value<CellDataBlock>();
	AbstractDataBlock adb(reinterpret_cast<MODBUS_U_WORD*>(&data));

    //! Write in data base
    m_adpator->Record(m_materialId,
						adb,
                      AbstractSqlTableAdpater::KEY_NAMED_KEY,
                      QVariant::fromValue(SlotBlock::ID));

    qDebug() << QString("%1,onUpdate elapsed,%2,%3").arg(m_slotIndex).arg(stopWatch.elapsed()).arg(m_materialId);
}

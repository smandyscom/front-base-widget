#include "controlleriomonitor.h"
#include <QSqlError>
ControllerIOMonitor::ControllerIOMonitor(quint8 clientId, Mode mode, QObject *parent) :
    ControllerBase(clientId,0,0,parent),
    m_mode(mode)
{
    //!serve only one model per controller
}

void ControllerIOMonitor::onAcknowledged(InterfaceRequest ack)
{
    ADDRESS_MODE ackAddress = ack.Address();
	quint8 clientId = ADDRESS_CLIENT_ID(ackAddress);
    //! Not my cared client
    if(clientId != m_clientId)
        return;
    //!
    //!
    //!
    findAndUpdate(ackAddress);
    //!
    //! If Polling
    switch (m_mode) {
    case POLLING:
    {
        int index  = m_addressList.indexOf(ackAddress);
        index++;
        if(index >= m_addressList.count())
            index =0;
        //!request feedback , boolean format query
		if(!m_addressList.isEmpty())
			m_channel->BeginRead(m_addressList[index],true);
        break;
    }
    default:
        break;
    }
    //! base method
    ControllerBase::onAcknowledged(ack);
}
//!
//! \brief ControllerIOMonitor::onDataChanged
//! \param topLeft
//! \param bottomRight
//! \param roles
//! When output operation coming
void ControllerIOMonitor::onDataChanged(const QModelIndex &topLeft,
                                        const QModelIndex &bottomRight,
                                        const QVector<int> &roles)
{
    //! Handling override request only
    if(roles.isEmpty())
        return;

    if (roles.first() != HEADER_STRUCTURE::UserRole_OverrideOnOff)
        return;
    //! Take address by name and set
    ADDRESS_MODE address = m_addressIndexTable.key(topLeft);
    //! Write-in
    m_channel->Access(address,topLeft.data(HEADER_STRUCTURE::UserRole_OverrideOnOff).toBool());
}

void ControllerIOMonitor::findAndUpdate(ADDRESS_MODE address)
{
    if(!m_addressIndexTable.contains(address))
        return;
    //! Direct access
    bool state = (*m_channel->Handle(address) & ADDRESS_BIT_ACCESSOR(address)) != 0;

    QModelIndex index = m_addressIndexTable[address].toModelIndex();

    //! Model user role
    bool result = m_model->setData(index,
                                   state,
                                   HEADER_STRUCTURE::UserRole_MonitorOnOff);
    if(!result)
    {
       qDebug() << m_model->lastError().text();
    }

//    qDebug() << result;
}

//!
//! \brief ControllerIOMonitor::setModel
//! \param model
//! \param addressNameMap
//! TODO , A register address could be map to multiple ModelIndex
void ControllerIOMonitor::setModel(QSqlTableModel* model,
                                   QMap<QVariant,QVariant> addressNameMap)
{
	m_model= model;
	m_addressNameMap = addressNameMap;
    //! Link override operation
    connect(m_model,&QSqlTableModel::dataChanged,this,&ControllerIOMonitor::onDataChanged);
	connect(m_model, &QSqlTableModel::modelReset, this, &ControllerIOMonitor::onModelReset);
	//!renew 
    m_model->setFilter(nullptr);
	//m_model->select();
    //!first shot
    m_channel->BeginRead(m_addressList.first(),false);
}

void ControllerIOMonitor::onModelReset()
{
	//!Reconstrut manifset
	m_addressList.clear();
	m_addressIndexTable.clear();

	for (int rowIndex = 0;
		rowIndex < m_model->rowCount();
		rowIndex++)
	{
		QMap<QVariant, QVariant>::ConstIterator i =
			m_addressNameMap.begin();
		while (i != m_addressNameMap.end()) {
			//! Select those record with Non-null name
			//! Use modelIndex

			//!Key :  Address
			//!Value : Name
			ADDRESS_MODE address = m_model->record(rowIndex).value(i.key().toString()).value<ADDRESS_MODE>();

			//bool isNull = m_model->record(rowIndex).value(i.value().toString()).isNull();
			int m_columnIndexName = m_model->fieldIndex(i.value().toString());

			if (!m_addressList.contains(address) /*&& !isNull*/)
			{
				m_addressList.append(address);
			}
			m_addressIndexTable[address] = m_model->index(rowIndex, m_columnIndexName); //link address and QModelIndex, or update
			//!Map

			i++;
		}//!while
	}//!for-row
}
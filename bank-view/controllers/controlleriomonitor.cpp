#include "controlleriomonitor.h"

ControllerIOMonitor::ControllerIOMonitor(quint8 clientId, Mode mode, QObject *parent) :
    ControllerBase(clientId,0,0,parent),
    m_mode(mode)
{
    //!serve only one model per controller
}

void ControllerIOMonitor::onAcknowledged(InterfaceRequest ack)
{
    ADDRESS_MODE ackAddress = ack.Address();

    //! Not my cared client
    if(ADDRESS_CLIENT_ID(ackAddress) != m_clientId)
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
        if(index >= m_addressList.count())
            index =0;
        else
            index++;
        //!request feedback
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
    if(roles.first() != HEADER_STRUCTURE::UserRole_OverrideOnOff)
        return;
    //! Take address by name and set
    ADDRESS_MODE address = m_addressNameTable.key(topLeft);
    //! Write-in
    m_channel->Access(address,topLeft.data(HEADER_STRUCTURE::UserRole_OverrideOnOff).toBool());
}

void ControllerIOMonitor::findAndUpdate(ADDRESS_MODE address)
{
    if(!m_addressNameTable.contains(address))
        return;
    //! Direct access
    bool state = *m_channel->Handle(address) & ADDRESS_BIT_ACCESSOR(address);
    //! Model user role
    m_model->setData(m_addressNameTable[address].value<QModelIndex>(),
                     state,
                     HEADER_STRUCTURE::UserRole_MonitorOnOff);
}

void ControllerIOMonitor::setModel(QSqlTableModel* model,
                                   QMap<QVariant,QVariant> addressNameMap)
{
    m_model= model;
    m_model->setFilter(nullptr);
    m_model->select();
    //! Link override operation
    connect(m_model,&QSqlTableModel::dataChanged,this,&ControllerIOMonitor::onDataChanged);

    for(int rowIndex=0;
        rowIndex < m_model->rowCount() ;
        rowIndex++)
    {
        QMap<QVariant,QVariant>::ConstIterator i =
            addressNameMap.begin();
        while (i != addressNameMap.end()) {
            //! Select those record with Non-null name
            //! Use modelIndex
            int m_columnIndexName = m_model->fieldIndex(i.value().toString());

            bool isNull = m_model->record(rowIndex).value(i.value().toString()).isNull();
            ADDRESS_MODE address = m_model->record(rowIndex).value(i.key().toString()).value<ADDRESS_MODE>();

            if(!m_addressList.contains(address) && !isNull)
            {
                m_addressList.append(address);
                m_addressNameTable[address] = m_model->index(rowIndex,m_columnIndexName);
            }
            i++;
        }//!while
    }//!for-row
}

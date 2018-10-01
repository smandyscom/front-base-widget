#include "controlleriomonitor.h"

ControllerIOMonitor::ControllerIOMonitor(int interval,QObject *parent) :
    ControllerBase(0,0,interval,parent)
{
    //!serve only one model per controller
}
//!
//! \brief ControllerIOMonitor::AttachReceiver
//! \param receiver
//! Scan intrested column
void ControllerIOMonitor::AttachReceiver(QObject *receiver)
{
     m_model = qobject_cast<QSqlTableModel*>(receiver);
     //! Link override operation
     connect(m_model,&QSqlTableModel::dataChanged,this,&ControllerIOMonitor::onDataChanged);

     QMap<QVariant,QVariant>::ConstIterator i =
             m_addressNameTable.begin();
     //! Pair scan
     while (i != m_addressNameTable.end()){
         int m_columnIndexAddress = m_model->fieldIndex(i.key().toString());
         //! Need to be pre-sorted
         m_model->sort(m_columnIndexAddress,Qt::AscendingOrder);
         //! Row by Row scan
         for(int rowIndex=0;rowIndex<m_model->rowCount();rowIndex++){
             ADDRESS_MODE address =
                     m_model->data(m_model->index(rowIndex,m_columnIndexAddress)).value<ADDRESS_MODE>();
             //! Check if within watch range
             bool isAnyWithin = false;
             foreach (ADDRESS_MODE var, m_channel->Routines().keys()){
                 //! client id matched , and within polling range
                 if(ADDRESS_CLIENT_ID(address) == ADDRESS_CLIENT_ID(var) &&
                    ADDRESS_REGISTER(address) < var + sizeof(CellDataBlock))
                     isAnyWithin |= true; //!within
             }//foreach

             //! Addin unoffseted
             if(!isAnyWithin)
                 m_channel->RegisterRoutines(address,QVariant::fromValue(CellDataBlock()),m_interval);
         }
         //!Increment
         i++;
     }//while
}

void ControllerIOMonitor::AppendPair(QVariant address, QVariant name)
{
    m_addressNameTable[address] = name;
}

void ControllerIOMonitor::onAcknowledged(InterfaceRequest ack)
{
    //! e.g 0x02080005
    QMap<QVariant,QVariant>::ConstIterator i =
            m_addressNameTable.begin();
    while (i != m_addressNameTable.end()) {
        //! Use modelIndex
        int m_columnIndexAddress = m_model->fieldIndex(i.key().toString());
        int m_columnIndexName = m_model->fieldIndex(i.value().toString());
        //! Row by Row
        for(int rowIndex=0;
            rowIndex < m_model->rowCount() ;
            rowIndex++)
        {
            ADDRESS_MODE address =
                    m_model->data(m_model->index(rowIndex,m_columnIndexAddress)).value<ADDRESS_MODE>();
            ADDRESS_MODE ackAddress = ack.Address();
            //!reject to procede if not cared address
            if((ADDRESS_CLIENT_ID(address) == ADDRESS_MODE(ackAddress)) &&
                    ADDRESS_REGISTER(address) < ackAddress + sizeof(CellDataBlock)){
                //! Direct access
                bool state = *m_channel->Handle(address) &
                        ADDRESS_BIT_ACCESSOR(address);
                //! Model user role
                m_model->setData(m_model->index(rowIndex,m_columnIndexName),
                                 state,
                                 Qt::UserRole);
            }//!if
        }
        //! Iterator increment
        i++;
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
    QString columnName = m_model->record().fieldName(topLeft.column());
    //! Take address by name and set
    ADDRESS_MODE address =
            m_model->record(topLeft.row()).value(m_addressNameTable.key(QVariant::fromValue(columnName)).toString()).toUInt();
    //! Write-in
    m_channel->Access(address,topLeft.data(HEADER_STRUCTURE::UserRole_OverrideOnOff).toBool());
}

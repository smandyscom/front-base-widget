#include "controlleriomonitor.h"

ControllerIOMonitor::ControllerIOMonitor(quint8 clientId,int interval,QObject *parent) :
    ControllerBase(clientId,0,interval,parent)
{
    //!TODO, serve only one model per controller

    //! Scan/Analyze I/O Table
    foreach (QObject* var, m_receivers) {
        //! Need to be pre-sorted
        m_model = qobject_cast<QSqlTableModel*>(var);

        QMap<QVariant,QVariant>::ConstIterator i =
                m_addressNameTable.begin();
        //! Pair scan
        while (i != m_addressNameTable.end()){
            m_columnIndexAddress = m_model->fieldIndex(i.key().toString());
            //! Row by Row scan
            for(int i=0;i<m_model->rowCount();i++){
                ADDRESS_MODE address =
                        m_model->data(m_model->index(i,m_columnIndexAddress)).value<ADDRESS_MODE>();
                //! Check if within watch range
                //! Shortage , continous memory block ,fragment cannot be detected
                foreach (ADDRESS_MODE var, m_baseAddressTable) {
                    if (!(ADDRESS_REGISTER(address) < var + sizeof(CellDataBlock)))
                    {
                         m_baseAddressTable.append(address);
                    }
                }
            }
            //!Increment
            i++;
        }//while
    }
    //! Addin watch list
    foreach (ADDRESS_MODE var, m_baseAddressTable) {
        registerWatchList(static_cast<ADDRESS_MODE>(var),
                          QVariant::fromValue(CellDataBlock()));
    }
}

void ControllerIOMonitor::onAcknowledged(InterfaceRequest ack)
{
    //!TODO, reject to procede if not cared address

    //! e.g 0x02080005
    foreach (QObject* var, m_receivers) {
        m_model = qobject_cast<QSqlTableModel*>(var);

        QMap<QVariant,QVariant>::ConstIterator i =
                m_addressNameTable.begin();
        while (i != m_addressNameTable.end()) {
            //! Use modelIndex
            m_columnIndexAddress = m_model->fieldIndex(i.key().toString());
            m_columnIndexName = m_model->fieldIndex(i.value().toString());
            //! Row by Row
            for(int rowIndex=0;
                rowIndex < m_model->rowCount() ;
                rowIndex++)
            {
                ADDRESS_MODE offsetedAddress =
                        m_model->index(rowIndex,
                                       m_columnIndexAddress).data().value<ADDRESS_MODE>();
                //! Direct access
                bool state = *m_channel->Handle(offsetedAddress) &
                        ADDRESS_BIT_ACCESSOR(offsetedAddress);

                m_model->setData(m_model->index(rowIndex,m_columnIndexName),
                                 state,
                                 Qt::UserRole);
            }
            //! Iterator increment
            i++;
        }
    }
    //! base method
    ControllerBase::onAcknowledged(ack);
}

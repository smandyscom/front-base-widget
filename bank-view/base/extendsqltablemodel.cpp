#include "extendsqltablemodel.h"

ExtendSqlTableModel::ExtendSqlTableModel(QObject *parent,QSqlDatabase db) :
    QSqlTableModel(parent,db),
    m_initialized(false)
{

}


bool ExtendSqlTableModel::select()
{
    bool result = QSqlTableModel::select();

    if(!m_initialized){
        for(int rowIndex=0;rowIndex<rowCount();rowIndex++)
        {
            m_monitorOnOff.append(QVector<bool>(columnCount()));
            m_overrideOnOff.append(QVector<bool>(columnCount()));
        }//for
        m_initialized = true; //lock
    }

    return result;
}

bool ExtendSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case HEADER_STRUCTURE::UserRole_MonitorOnOff:
        m_monitorOnOff[index.row()][index.column()] = value.toBool();
        emit dataChanged(index,index,QVector<int>{role});
        return true;
    case HEADER_STRUCTURE::UserRole_OverrideOnOff:
        m_overrideOnOff[index.row()][index.column()] = value.toBool();
        emit dataChanged(index,index,QVector<int>{role});
        return true;
    default:
        return QSqlTableModel::setData(index,value,role);
    }
}

QVariant ExtendSqlTableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case HEADER_STRUCTURE::UserRole_MonitorOnOff:
        return m_monitorOnOff[index.row()][index.column()];
    case HEADER_STRUCTURE::UserRole_OverrideOnOff:
        return m_overrideOnOff[index.row()][index.column()];
    default:
        return QSqlTableModel::data(index,role);
    }
}

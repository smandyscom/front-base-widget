#include "junctionbankdatabase.h"
#include <QDebug>
#include <QApplication>
#include <QMetaEnum>
JunctionBankDatabase::JunctionBankDatabase(QString databaseName, QObject *parent) :
    QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE","base");//setup driver
    m_database.setDatabaseName(databaseName);
}
JunctionBankDatabase::~JunctionBankDatabase()
{
    m_database.close();
}

void JunctionBankDatabase::onInitialize()
{
    QFileInfo qf(__databaseName);
    qDebug() << QDir::currentPath();

    if(!qf.exists())
        return;

    if(!m_database.open())
        return;

    qDebug() << qf.absoluteFilePath();

    //! Table name preparation
    QList<TableNames> tableList;
    QMetaEnum qme  =QMetaEnum::fromType<TableNames>();
    for(int i=0;i<qme.keyCount();i++)
    {
        tableList.append(TableNames(qme.value(i)));
    }
    //! Open tables
    foreach (TableNames var, tableList) {
        QSqlTableModel* m_reference = new QSqlTableModel(this,m_database);
        bool result = false;
        m_reference->setEditStrategy(QSqlTableModel::OnFieldChange);
        m_reference->setTable(QVariant::fromValue(var).value<QString>());
        result = m_reference->select();
        m_tableMap[var] = TableEntity(result,m_reference);
        //! Output results
        qDebug() << QString("%1:%2")
                        .arg(QVariant::fromValue(var).value<QString>())
                        .arg(result);

    }


    emit databaseOpened();
}
//!
//! \brief JunctionBankDatabase::onReleaseHeaders
//! Release header tables
void JunctionBankDatabase::onReleaseHeaders()
{

}
//!
//! \brief JunctionBankDatabase::TableMap
//! \param value
//! \return
//! Not duplicate new table
//! Resource control
QSqlTableModel* JunctionBankDatabase::TableMap(TableNames value)
{
    QSqlTableModel* result = nullptr;

    if(m_tableMap.contains(value) && m_tableMap[value].first)
        result = m_tableMap[value].second;

    return result;
}

JunctionBankDatabase* JunctionBankDatabase::Instance()
{
    if(__instance == nullptr)
        __instance = new JunctionBankDatabase(__databaseName,qApp);
    return __instance;
}

JunctionBankDatabase* JunctionBankDatabase::__instance = nullptr;
QString JunctionBankDatabase::__databaseName = "base.db";

#include "junctionbankdatabase.h"
#include <QDebug>
#include <QApplication>
#include <QMetaEnum>
JunctionBankDatabase::JunctionBankDatabase(QString databaseName, QObject *parent) :
    QObject(parent)
{
    __database = QSqlDatabase::addDatabase("QSQLITE","base");//setup driver
    __database.setDatabaseName(databaseName);
}
JunctionBankDatabase::~JunctionBankDatabase()
{
    __database.close();
}

void JunctionBankDatabase::onInitialize()
{
    QFileInfo qf(__databaseName);
    qDebug() << QDir::currentPath();

    if(!qf.exists())
        return;

    if(!__database.open())
        return;

    qDebug() << qf.absoluteFilePath();

    //! Table name preparation
    QList<TableNames> __tableList;
    QMetaEnum __qme  =QMetaEnum::fromType<TableNames>();
    for(int i=0;i<__qme.keyCount();i++)
    {
        __tableList.append(TableNames(__qme.value(i)));
    }
    //! Open tables
    foreach (TableNames var, __tableList) {
        QSqlTableModel* __reference = new QSqlTableModel(this,__database);
        bool result = false;
        __reference->setEditStrategy(QSqlTableModel::OnFieldChange);
        __reference->setTable(QVariant::fromValue(var).value<QString>());
//        result = __reference->select();
        __tableMap[var] = TableEntity(false,__reference);
//        //! Output results
//        qDebug() << QString("%1:%2")
//                        .arg(QVariant::fromValue(var).value<QString>())
//                        .arg(result);

    }


    emit databaseOpened();
}

JunctionBankDatabase* JunctionBankDatabase::Instance()
{
    if(__instance == nullptr)
        __instance = new JunctionBankDatabase(__databaseName,qApp);
    return __instance;
}

JunctionBankDatabase* JunctionBankDatabase::__instance = nullptr;
QString JunctionBankDatabase::__databaseName = "base.db";

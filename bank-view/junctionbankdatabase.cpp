#include "junctionbankdatabase.h"
#include <QDebug>
#include <QApplication>
#include <QMetaEnum>
JunctionBankDatabase::JunctionBankDatabase(QString databaseName, QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");//setup driver
    db.setDatabaseName(databaseName);
}
JunctionBankDatabase::~JunctionBankDatabase()
{
    db.close();
}

void JunctionBankDatabase::onInitialize()
{
    QFileInfo qf(__databaseName);
    qDebug() << QDir::currentPath();

    if(!qf.exists())
        return;

    if(!db.open())
        return;

    //! Table name preparation
    QList<TableNames> __tableList;
    QMetaEnum __qme  =QMetaEnum::fromType<TableNames>();
    for(int i=0;i<__qme.keyCount();i++)
    {
        __tableList.append(TableNames(__qme.value(i)));
    }
    //! Open tables
    foreach (TableNames var, __tableList) {
        QSqlRelationalTableModel* __reference = new QSqlRelationalTableModel(this);
        bool result = false;
        __reference->setEditStrategy(QSqlTableModel::OnFieldChange);
        __reference->setTable(QVariant::fromValue(var).value<QString>());
        result = __reference->select();
        __tableMap[var] = TableEntity(result,new AbstractQVariantSqlTable(__reference));
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

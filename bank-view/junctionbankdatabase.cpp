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

    //! Prepared models
    __axisTable = new QSqlRelationalTableModel(this);
    __axisTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    __axisTable->setTable(QVariant::fromValue(WHOLE_AXIS).value<QString>());
    __axisTable->select();//engaged
    //!
    __commandBlockTable = new QSqlRelationalTableModel(this);
    __commandBlockTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    __commandBlockTable->setTable(QVariant::fromValue(WHOLE_COMMAND_BLOCKS).value<QString>());
    __commandBlockTable->select();//engaged
    //!
    __inputTable = new TableModelIOOverride(this);
    __inputTable->setTable(QVariant::fromValue(INPUT_ATTRIBUTES).value<QString>());
    __inputTable->select();//engaged
    //!
    __outputTable = new TableModelIOOverride(this);
    __outputTable->setTable(QVariant::fromValue(OUTPUT_ATTRIBUTES).value<QString>());
    __outputTable->select();//engaged
    //!
    __regionTable = new QSqlRelationalTableModel(this);
    __regionTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    __regionTable->setTable(QVariant::fromValue(DEF_REGION).value<QString>());
    __regionTable->select();//engaged

//    QMetaEnum __qme  =QMetaEnum::fromType<TableNames>();
//    for(int i=0;i<__qme.keyCount();i++)
//    {
//        QSqlRelationalTableModel* reference = new QSqlRelationalTableModel(this);
//        bool result = false;
//        reference->setEditStrategy(QSqlTableModel::OnManualSubmit);
//        reference->setTable(QVariant::fromValue(TableNames(__qme.value(i))).value<QString>());
//        result = reference->select();
//        __tableMap[TableNames(__qme.value(i))] = TableEntity(result,reference);
//    }

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

#include "junctionbankdatabase.h"

JunctionBankDatabase::JunctionBankDatabase(QString databaseName, QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");//setup driver
    db.setDatabaseName(databaseName);
}

void JunctionBankDatabase::onInitialize()
{
    if(!db.open())
        return;

    //! Prepared models
    __axisTable = new QSqlTableModel(this);
    __axisTable->setTable(QVariant::fromValue(WHOLE_AXIS).value<QString>());
    __axisTable->select();//engaged

    __commandBlockTable = new QSqlTableModel(this);
    __commandBlockTable->setTable(QVariant::fromValue(WHOLE_COMMAND_BLOCKS).value<QString>());
    __commandBlockTable->select();//engaged

    emit databaseOpened();
}

JunctionBankDatabase* JunctionBankDatabase::Instance()
{
    if(__instance == nullptr)
        __instance = new JunctionBankDatabase(__databaseName);
}

JunctionBankDatabase* JunctionBankDatabase::__instance = nullptr;
QString JunctionBankDatabase::__databaseName = "base.db";

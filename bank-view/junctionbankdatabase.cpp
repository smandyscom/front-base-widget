#include "junctionbankdatabase.h"
#include <QDebug>
#include <QApplication>

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

    __commandBlockTable = new QSqlRelationalTableModel(this);
    __commandBlockTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    __commandBlockTable->setTable(QVariant::fromValue(WHOLE_COMMAND_BLOCKS).value<QString>());
    __commandBlockTable->select();//engaged

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

#ifndef JUNCTIONBANKDATABASE_H
#define JUNCTIONBANKDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QAbstractTableModel>
#include <QFileInfo>
#include <QDir>

//!
//! \brief The JunctionBankDatabase class
//! Should be singleton
class JunctionBankDatabase : public QObject
{


    Q_OBJECT
public:


    enum TableNames
    {
        WHOLE_AXIS,
        WHOLE_COMMAND_BLOCKS,
    };
    Q_ENUM(TableNames)


    static JunctionBankDatabase* Instance();
    static void DatabaseName(QString value){ __databaseName = value;}
    static QString DatabaseName(){return __databaseName;}

    QSqlTableModel* AxisTable() const {return __axisTable;}
    QSqlTableModel* CommandBlockTable() const {return __commandBlockTable;}

signals:
    void databaseOpened();
public slots:
    void onInitialize();
protected:
    static JunctionBankDatabase* __instance;
    static QString __databaseName;

    explicit JunctionBankDatabase(QString databaseName,QObject *parent = nullptr);
    ~JunctionBankDatabase();

    QSqlDatabase db;

    QSqlTableModel* __axisTable;
    QSqlTableModel* __commandBlockTable;
};

#endif // JUNCTIONBANKDATABASE_H

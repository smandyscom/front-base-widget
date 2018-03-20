#ifndef JUNCTIONBANKDATABASE_H
#define JUNCTIONBANKDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QAbstractTableModel>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QPair>
#include <tablemodeliooverride.h>
//!
//! \brief TableEntity
//!
typedef QPair<bool,QSqlRelationalTableModel*> TableEntity;

//!
//! \brief The JunctionBankDatabase class
//! Should be singleton
class JunctionBankDatabase : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief The TableNames enum
    //! Those tables this application relied on
    enum TableNames
    {
        WHOLE_AXIS,
        WHOLE_COMMAND_BLOCKS,
        DEF_REGION,
        DEF_IS_ABS,
        DEF_MIII_ZRET_METHOD,
        INPUT_ATTRIBUTES,
        OUTPUT_ATTRIBUTES,
    };
    Q_ENUM(TableNames)


    static JunctionBankDatabase* Instance();
    static void DatabaseName(QString value){ __databaseName = value;}
    static QString DatabaseName(){return __databaseName;}

    QSqlRelationalTableModel* AxisTable() const {return __axisTable;}
    QSqlRelationalTableModel* CommandBlockTable() const {return __commandBlockTable;}
    QSqlRelationalTableModel* InputTable() const { return __inputTable;}
    QSqlRelationalTableModel* OutputTable() const {return __outputTable;}
    QSqlRelationalTableModel* RegionTable() const {return __regionTable;}
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

    QMap<TableNames,TableEntity> __tableMap;

    QSqlRelationalTableModel* __axisTable;
    QSqlRelationalTableModel* __commandBlockTable;
    QSqlRelationalTableModel* __inputTable;
    QSqlRelationalTableModel* __outputTable;
    QSqlRelationalTableModel* __regionTable;
};

#endif // JUNCTIONBANKDATABASE_H

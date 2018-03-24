#ifndef JUNCTIONBANKDATABASE_H
#define JUNCTIONBANKDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QPair>
#include <tablemodeliooverride.h>
#include <tablemodelcylinder.h>
#include <abstractqvariantsqltable.h>
#include <tablemodelcommandblock.h>

#include <QList>
//!
//! \brief TableEntity
//!
typedef QPair<bool,AbstractQVariantSqlTable*> TableEntity;

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
        WHOLE_CYLINDERS,
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
    TableModelCommandBlock* CommandBlockTable() const {return reinterpret_cast<TableModelCommandBlock*>(__commandBlockTable);}
    QSqlRelationalTableModel* InputTable() const { return __inputTable;}
    QSqlRelationalTableModel* OutputTable() const {return __outputTable;}
    QSqlRelationalTableModel* RegionTable() const {return __regionTable;}


    AbstractQVariantSqlTable* TableMap(TableNames value) const
    {
        return __tableMap[value].second;
    }

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
    AbstractQVariantSqlTable* __commandBlockTable;
    AbstractQVariantSqlTable* __inputTable;
    AbstractQVariantSqlTable* __outputTable;
    AbstractQVariantSqlTable* __regionTable;
};

#endif // JUNCTIONBANKDATABASE_H

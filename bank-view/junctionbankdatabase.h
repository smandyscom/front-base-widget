#ifndef JUNCTIONBANKDATABASE_H
#define JUNCTIONBANKDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QPair>
#include <QList>

#include <QSqlRelationalTableModel>.h>

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
        //! Data tables
        WHOLE_AXIS,
        WHOLE_CYLINDERS,
        WHOLE_SIGNALS,
        WHOLE_COMMAND_BLOCKS,
        WHOLE_UNITS,
        //! Material
        WHOLE_MAT_DATA_SLOTN,
        //! Reference tables
        DEF_REGION,
        DEF_IS_ABS,
        DEF_MIII_ZRET_METHOD,
        //! I/O Tables
        INPUT_ATTRIBUTES,
        OUTPUT_ATTRIBUTES,
    };
    Q_ENUM(TableNames)


    static JunctionBankDatabase* Instance();
    static void DatabaseName(QString value){ __databaseName = value;}
    static QString DatabaseName(){return __databaseName;}

    QSqlRelationalTableModel* TableMap(TableNames value) const
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

    virtual void decorateTable(){}

    QSqlDatabase db;

    QMap<TableNames,TableEntity> __tableMap;

};

#endif // JUNCTIONBANKDATABASE_H

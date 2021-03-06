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

#include<QDebug>
//!
//! \brief TableEntity
//!
typedef QPair<bool,QSqlTableModel*> TableEntity;

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
        //!
        DEF_OBJECT_TYPE,
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
        DEF_EXT_CTRL_BIT_0,
        DEF_UNIT_TYPE,
        DEF_COMMAND_TYPE,
        DEF_BASIC_DIMENSION,
        DEF_AXIS_TYPE,
        DEF_AUTH,
        //! ERROR CODES
        ERROR_CODE_AXIS,
        ERROR_CODE_CYLINDER,
        ERROR_CODE_UNIT,
        //! MIII tables
        DEF_MIII_ZRET_METHOD,
        //! I/O Tables
        WHOLE_INPUTS,
        WHOLE_OUTPUTS,
        //! Header
        HEADER_AXIS,
        HEADER_CYLINDERS,
        HEADER_SIGNALS,
        HEADER_COMMAND_BLOCKS,
        HEADER_UNIT,
        HEADER_IO,
    };
    Q_ENUM(TableNames)


    static JunctionBankDatabase* Instance();
    static void DatabaseName(QString value){ __databaseName = value;}
    static QString DatabaseName(){return __databaseName;}

    QSqlTableModel* TableMap(TableNames value)
    {
        QSqlTableModel* result = new QSqlTableModel(this,__database);
        result->setEditStrategy(QSqlTableModel::OnFieldChange);
        result->setTable(__tableMap[value].second->tableName());

        bool bResult = result->select();
        __tableMap[value].first = bResult;
        //! Output results
        qDebug() << QString("%1:%2")
                        .arg(QVariant::fromValue(value).value<QString>())
                        .arg(bResult);
        return result;
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

    QSqlDatabase __database;

    QMap<TableNames,TableEntity> __tableMap;

};

#endif // JUNCTIONBANKDATABASE_H

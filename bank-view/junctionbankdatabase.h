#ifndef JUNCTIONBANKDATABASE_H
#define JUNCTIONBANKDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractTableModel>
//!
//! \brief The JunctionBankDatabase class
//! Should be singleton
class JunctionBankDatabase : public QObject
{
    enum AxisTableHeaders
    {
        ATH_ID,
        ATH_REGION,
        ATH_NAME,
        ATH_ADDRESS,
        ATH_TYPE,
        ATH_LIMIT_MINUS,
        ATH_LIMIT_PLUS,
        ATH_POSITION_TOLERANCE,
        ATH_SPEED_MAX,
        ATH_COMMENT,
    };
    Q_ENUM(AxisTableHeaders)
    enum CommandBlockTableHeaders
    {
        CBT_AXIS_ID,
        CBT_BANK_ID,
        CBT_NAME,
        CBT_IS_ABS,
        CBT_COORDIANTE,
        CBT_SPEED,
        CBT_ACCERALATION,
        CBT_DCCERALATION,
        CBT_TORQUE_LIMIT,
        CBT_COMMENT,
    };
    Q_ENUM(CommandBlockTableHeaders)
    enum TableNames
    {
        WHOLE_AXIS,
        WHOLE_COMMAND_BLOCKS,
    };
    Q_ENUM(TableNames)

    Q_OBJECT
public:
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

    QSqlDatabase db;

    QSqlTableModel* __axisTable;
    QSqlTableModel* __commandBlockTable;
};

#endif // JUNCTIONBANKDATABASE_H

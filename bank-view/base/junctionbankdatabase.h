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

#include <QSqlTableModel>
#include <extendsqltablemodel.h>
#include <debugsqltablemodel.h>
#include <junctioncommondatabase.h>

#include<QDebug>

//!
//! \brief The JunctionBankDatabase class
//! Should be singleton
class JunctionBankDatabase : 
	public JunctionCommonDatabase
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
		CLIENTS_CONFIG,
		CONTROLLER_MONITOR, //structured
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
		//! Safe I/O
		WHOLE_SAFETY_INPUTS,
		WHOLE_SAFETY_OUTPUTS,
        //! Header
        HEADER_AXIS,
        HEADER_CYLINDERS,
        HEADER_SIGNALS,
        HEADER_COMMAND_BLOCKS,
        HEADER_UNIT,
        HEADER_IO,
		HEADER_MONITOR,
		//
		PROPERTIES,
    };
    Q_ENUM(TableNames)


    static JunctionBankDatabase* Instance();
    static void DatabaseName(QString value){ m_databaseName = value;}
    static QString DatabaseName(){return m_databaseName;}

protected slots:
    bool onOpenTables() Q_DECL_OVERRIDE;
protected:
    static JunctionBankDatabase* m_instance;
    static QString m_databaseName;

    explicit JunctionBankDatabase(QObject *parent = nullptr);
    ~JunctionBankDatabase();

	//! implemented by derived class
	QList<QVariant> onGenerateTableNames() Q_DECL_OVERRIDE;
};

#endif // JUNCTIONBANKDATABASE_H

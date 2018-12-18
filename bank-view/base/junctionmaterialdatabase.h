#pragma once

#include <QObject>
#include <junctioncommondatabase.h>

class JunctionMaterialDatabase : 
	public JunctionCommonDatabase
{
	Q_OBJECT

public:
	enum TableNames
	{
		MAT_DATA_SLOT,
		MAT_HEADER_SLOT,
		//------------
		QUERY_HEAD,

	};
	Q_ENUM(TableNames)

	JunctionMaterialDatabase(QObject *parent=nullptr);
	~JunctionMaterialDatabase();

	static JunctionMaterialDatabase* Instance();
	static void DatabaseName(QString value) { m_databaseName = value; }
	static QString DatabaseName() { return m_databaseName; }

	//!Overload function to select slot tables
	QSqlTableModel* TableMap(int index,TableNames key= MAT_DATA_SLOT);

protected slots:
protected:
	static JunctionMaterialDatabase* m_instance;
	static QString m_databaseName;

	//! implemented by derived class
	QList<QVariant> onGenerateTableNames() Q_DECL_OVERRIDE;
};

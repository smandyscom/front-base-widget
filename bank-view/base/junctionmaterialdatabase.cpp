#include "junctionmaterialdatabase.h"
#include <qapplication.h>
#include <QMetaEnum>
#include <utilities.h>

JunctionMaterialDatabase::JunctionMaterialDatabase(QObject *parent)
	: JunctionCommonDatabase(m_databaseName,parent)
{

}

JunctionMaterialDatabase::~JunctionMaterialDatabase()
{

}

JunctionMaterialDatabase* JunctionMaterialDatabase::Instance()
{
	if (m_instance == nullptr)
		m_instance = new JunctionMaterialDatabase(qApp);
	return m_instance;
}

QSqlTableModel* JunctionMaterialDatabase::TableMap(int index, TableNames key)
{
	QVariant p_key = QString("%1%2")
		.arg(QVariant::fromValue(p_key).toString())
		.arg(index);

	//!Query if existed
	if (m_tableMap.contains(p_key))
		return m_tableMap[p_key].second;
	else
	{
		//not existed , open table
		QSqlTableModel* ref = new QSqlTableModel(this, m_database);
		ref->setTable(p_key.toString());
		if (ref->select())
			m_tableMap[p_key] = TableEntity(true, ref);
		else
			return nullptr; //not existed
	}
}

JunctionMaterialDatabase* JunctionMaterialDatabase::m_instance = nullptr;
QString JunctionMaterialDatabase::m_databaseName = "base.db";

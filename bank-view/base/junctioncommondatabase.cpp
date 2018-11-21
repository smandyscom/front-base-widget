#include "junctioncommondatabase.h"

#include <qfileinfo.h>
#include <qdir>
#include <debugsqltablemodel.h>
#include <qdebug.h>

JunctionCommonDatabase::JunctionCommonDatabase(QString databaseName, QObject *parent)
	: QObject(parent) ,
	m_initialized(false)
{
	m_database = QSqlDatabase::addDatabase("QSQLITE", databaseName);//setup driver
	m_database.setDatabaseName(databaseName);

	//initializing and loading interested tables
	onInitialize();
	onOpenTables();
}

JunctionCommonDatabase::~JunctionCommonDatabase()
{
	m_database.close();
}

bool JunctionCommonDatabase::onInitialize()
{
	QFileInfo qf(m_database.databaseName());
	qDebug() << QDir::currentPath();

	if (!qf.exists())
		return false;

	if (!m_database.open())
		return false;

	qDebug() << qf.absoluteFilePath();

	return true;
}

bool JunctionCommonDatabase::onOpenTables()
{
	QList<QVariant> m_tableNames = onGenerateTableNames();

	for each (QVariant var in m_tableNames)
	{
		QSqlTableModel* m_reference = new DebugSqlTableModel(this, m_database);

		
		m_reference->setEditStrategy(QSqlTableModel::OnFieldChange);
		m_reference->setTable(QVariant::fromValue(var).value<QString>());
		m_tableMap[var] = TableEntity(m_reference->select(), m_reference);
		//! Output results
		qDebug() << QString("%1:%2")
			.arg(QVariant::fromValue(var).value<QString>())
			.arg(m_tableMap[var].first);
	}

	QList<TableEntity>::Iterator it = std::find_if(m_tableMap.values().begin(),
		m_tableMap.values().end(),
		[=](TableEntity i) {
		return !i.first;
	});
	
	//! when nothing searched return last
	return it == m_tableMap.values().end();
}

QList<QVariant> JunctionCommonDatabase::onGenerateTableNames()
{
	//! implemented by derived class
	return QList<QVariant>();
}

QSqlTableModel* JunctionCommonDatabase::TableMap(QVariant value)
{
	QSqlTableModel* result = nullptr;

	if (m_tableMap.contains(value) && m_tableMap[value].first)
		result = m_tableMap[value].second;

	return result;
}

bool JunctionCommonDatabase::Initialized() const
{
	return m_initialized;
}
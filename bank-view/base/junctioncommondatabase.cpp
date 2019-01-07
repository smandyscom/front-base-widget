#include "junctioncommondatabase.h"

#include <qfileinfo.h>
#include <qdir>
#include <debugsqltablemodel.h>
#include <qdebug.h>
#include <algorithm>

JunctionCommonDatabase::JunctionCommonDatabase(QString databaseName, QObject *parent)
	: QObject(parent) ,
	m_initialized(false)
{
	m_database = QSqlDatabase::addDatabase("QSQLITE", databaseName);//setup driver
	m_database.setDatabaseName(databaseName);

	//initializing and loading interested tables
	onInitialize();
}

JunctionCommonDatabase::~JunctionCommonDatabase()
{
	m_database.close();
}

bool JunctionCommonDatabase::onInitialize()
{
	QFileInfo qf(m_database.databaseName());
	qDebug() << QDir::currentPath();
	
	/*QDir d(QDir::currentPath());
	QStringList l = d.entryList();
	std::for_each(l.begin(),
		l.end(),
		[](QString s) {
		qDebug() << s;
	});*/


	if (!qf.exists())
	{
		qf.setFile(QString("%1.lnk").arg(qf.fileName()));
		//!try if link existed
		if (!qf.exists())
			return false;
		m_database.setDatabaseName(qf.symLinkTarget()); //link to target
	}

	if (!m_database.open())
		return false;

	qDebug() << QString("%1 opened with %2 tables").arg(m_database.databaseName()).arg(m_database.tables().count());

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

	bool result = true;
	for each (TableEntity var in m_tableMap.values())
	{
		result &= var.first;
	}


	//!make sure all result are true
	return result;
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
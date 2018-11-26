#pragma once

#include <QObject>
#include <qvariant.h>
#include <qsqltablemodel.h>
#include <qsqldatabase.h>

//!
//! \brief TableEntity
//! whether opened,coresponding table
typedef QPair<bool, QSqlTableModel*> TableEntity;

class JunctionCommonDatabase : public QObject
{
	Q_OBJECT

public:
	JunctionCommonDatabase(QString databaseName, QObject *parent);
	~JunctionCommonDatabase();

	QSqlTableModel* TableMap(QVariant key);
	bool Initialized() const;

protected slots:
	virtual bool onInitialize();
	virtual bool onOpenTables();
protected:
	QSqlDatabase m_database;
	QMap<QVariant, TableEntity> m_tableMap;

	bool m_initialized;
	//! implemented by derived class
	virtual QList<QVariant> onGenerateTableNames() = 0;

};

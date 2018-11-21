#include "junctionbankdatabase.h"
#include <QDebug>
#include <QApplication>
#include <QMetaEnum>
JunctionBankDatabase::JunctionBankDatabase(QObject *parent) :
	JunctionCommonDatabase(m_databaseName,parent)
{
}
JunctionBankDatabase::~JunctionBankDatabase()
{
}

bool JunctionBankDatabase::onOpenTables()
{
	bool result = JunctionCommonDatabase::onOpenTables();

	QList<QVariant> extends{ 
		QVariant::fromValue(WHOLE_CYLINDERS),
		QVariant::fromValue(WHOLE_INPUTS),
		QVariant::fromValue(WHOLE_OUTPUTS) 
	};

	//! instance replace
	for each (QVariant var in extends)
	{
		delete m_tableMap[var].second; //release
		ExtendSqlTableModel* replace = new ExtendSqlTableModel(this, m_database);
		replace->setTable(var.toString());

		m_tableMap[var].first = replace->select();
		m_tableMap[var].second = replace;

	}

	return result;
}

JunctionBankDatabase* JunctionBankDatabase::Instance()
{
    if(m_instance == nullptr)
        m_instance = new JunctionBankDatabase(qApp);
    return m_instance;
}

QList<QVariant> JunctionBankDatabase::onGenerateTableNames()
{
	return utilities::listupEnumVariant<TableNames>();
}

JunctionBankDatabase* JunctionBankDatabase::m_instance = nullptr;
QString JunctionBankDatabase::m_databaseName = "base.db";

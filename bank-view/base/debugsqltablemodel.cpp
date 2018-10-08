#include "debugsqltablemodel.h"
#include <qdebug.h>

DebugSqlTableModel::DebugSqlTableModel(QObject *parent, QSqlDatabase db)
	: QSqlTableModel(parent,db)
{
}

DebugSqlTableModel::~DebugSqlTableModel()
{
}
void DebugSqlTableModel::printLastError()
{
	qDebug() << QString("%1,%2")
		.arg(objectName())
		.arg(lastError().text());
}
void DebugSqlTableModel::setTable(const QString &tableName)
{
	setObjectName(tableName);
	QSqlTableModel::setTable(tableName);
}

bool DebugSqlTableModel::select()
{
	bool result = QSqlTableModel::select();

	if (!result)
		printLastError();

	return result;
}

bool DebugSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool result = QSqlTableModel::setData(index, value, role);

	if (!result)
		printLastError();

	return result;
}

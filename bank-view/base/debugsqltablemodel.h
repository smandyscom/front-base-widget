#pragma once

#include <QSqlTableModel>
#include <qsqlerror.h>

class DebugSqlTableModel : 
	public QSqlTableModel
{
	Q_OBJECT

public:
	DebugSqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
	~DebugSqlTableModel();

	bool select() Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

	void setTable(const QString &tableName) Q_DECL_OVERRIDE;

protected:

	void printLastError();
};

#ifndef EXTENDSQLTABLEMODEL_H
#define EXTENDSQLTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>

#include <debugsqltablemodel.h>
#include <definitionauxiliarykeys.h>

//!
//! \brief The ExtendSqlTableModel class
//! With user role storage and handling method
class ExtendSqlTableModel :
        public DebugSqlTableModel
{
    Q_OBJECT
public:
    explicit ExtendSqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    bool select() Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &idx, int role) const Q_DECL_OVERRIDE;

signals:

public slots:
protected:
    bool m_initialized;
    QVector<QVector<bool>> m_monitorOnOff;
    QVector<QVector<bool>> m_overrideOnOff;
};

#endif // EXTENDSQLTABLEMODEL_H

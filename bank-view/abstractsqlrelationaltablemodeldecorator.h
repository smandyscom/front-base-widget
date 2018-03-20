#ifndef ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H
#define ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H

#include <QObject>
#include <QSqlRelationalTableModel>
//!
//! \brief The AbstractSqlRelationalTableModelDecorator class
//! The decorator base
class AbstractSqlRelationalTableModelDecorator : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit AbstractSqlRelationalTableModelDecorator(QSqlRelationalTableModel* instance);

    QSqlRelationalTableModel* Instance() const {return __instance;}


    //! Ovrrides
    void setFilter(const QString &filter) Q_DECL_OVERRIDE {__instance->setFilter(filter);}
    bool select() Q_DECL_OVERRIDE { __instance->select();}
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE { return __instance->rowCount();}
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE {return __instance->data(idx,role);}
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE {return __instance->setData(index,value,role);}
signals:

public slots:

protected:
    QSqlRelationalTableModel* __instance;
};

#endif // ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H

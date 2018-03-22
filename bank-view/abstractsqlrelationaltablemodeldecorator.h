#ifndef ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H
#define ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H

#include <QObject>
#include <QSqlRelationalTableModel>
//!
//! \brief The AbstractSqlRelationalTableModelDecorator class
//! The decorator base
template <typename T>
class AbstractEnhancedSqlRelationalTable : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit AbstractEnhancedSqlRelationalTable<T>(QObject *parent = Q_NULLPTR,
                                                QSqlDatabase db = QSqlDatabase())
    {

    }


    //! Additional function

signals:

public slots:

};

#endif // ABSTRACTSQLRELATIONALTABLEMODELDECORATOR_H

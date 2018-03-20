#include "abstractsqlrelationaltablemodeldecorator.h"

AbstractSqlRelationalTableModelDecorator::AbstractSqlRelationalTableModelDecorator(QSqlRelationalTableModel *instance) :
    QSqlRelationalTableModel(instance->parent())
{

}

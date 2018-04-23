#include "utilities.h"

utilities::utilities()
{

}

KeyValueCollection utilities::model2KeyValuePairs(const QAbstractItemModel &model)
{
    KeyValueCollection output;

    //takes first column as key , second as value
    for(int i=0;i<model.rowCount();i++)
        output.append(KeyValuePair(model.data(model.index(i,0)),
                                   model.data(model.index(i,1))));

    return output;
}

int utilities::sizeOf(QVariant value)
{
    return QMetaType::sizeOf(QMetaType::type(value.typeName()));
}

void utilities::colorChangeOver(QWidget *target, bool value, Qt::GlobalColor trueColor)
{
    Qt::GlobalColor __color = trueColor;
    if(!value)
        __color = Qt::gray;
    target->setStyleSheet(QString("background-color : %1").arg(QVariant::fromValue(__color).value<QString>()));
}

int utilities::getSelectedValue(QTableView *target, const QString &keyName)
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(target->model());
    return  model->record(target->selectionModel()->selectedRows().first().row()).value(keyName).toInt();
}

QSqlRecord utilities::getSqlTableSelectedRecord(QSqlTableModel *target, QVariant keyName, QVariant keyValue)
{
    QString origin = target->filter();
    target->setFilter(generateFilterString(trimNamespace(keyName),trimNamespace(keyValue)));
    QSqlRecord result = target->record(0);
    target->setFilter(origin);
    return result;
}

QString utilities::generateFilterString(QVariant keyName, QVariant keyValue)
{
    return QString("%1=\'%2\'").arg(trimNamespace(keyName)).arg(trimNamespace(keyValue));
}

QString utilities::trimNamespace(QVariant key)
{
    return key.toString().split("::").last();
}

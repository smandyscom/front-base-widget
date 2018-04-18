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

void utilities::getSelectedValue(QTableView *target, const QString &keyName)
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(target->model());
    return  model->record(target->selectionModel()->selectedRows().first()).value(keyName).toInt();
}

QSqlRecord utilities::getSqlTableSelectedRecord(QSqlTableModel *target, QVariant keyName, QVariant keyValue)
{
    QString origin = target->filter();
    target->setFilter(QString("%1=\'%2\'").arg(keyName.toString()).arg(keyValue.toString()));
    QSqlRecord result = target->record();
    target->setFilter(origin);
    return result;
}

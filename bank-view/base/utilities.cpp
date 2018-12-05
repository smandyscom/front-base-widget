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

void utilities::colorChangeOver(QWidget *target, bool value,
                                Qt::GlobalColor trueColor,
                                Qt::GlobalColor falseColor)
{
    Qt::GlobalColor __color = trueColor;
    if(!value)
        __color = falseColor;
    target->setStyleSheet(QString("background-color : %1").arg(QVariant::fromValue(__color).value<QString>()));
}

int utilities::getSelectedValue(QTableView *target, const QString &keyName)
{
    QSqlTableModel* model = qobject_cast<QSqlTableModel*>(target->model());
    return  model->record(target->selectionModel()->selectedRows().first().row()).value(keyName).toInt();
}

QSqlRecord utilities::getSqlTableSelectedRecord(QSqlTableModel *target, QVariant keyName, QVariant keyValue)
{
    //!append one record if there is not existed in cache
    if(!m_cachedTables.contains(target->database().connectionName()))
        m_cachedTables[target->database().connectionName()] = new QSqlTableModel(nullptr,target->database());

    QSqlTableModel* m_dup = m_cachedTables[target->database().connectionName()];

    m_dup->setTable(target->tableName());
    m_dup->select();

    m_dup->setFilter(generateFilterString(trimNamespace(keyName),trimNamespace(keyValue)));
    QSqlRecord result = m_dup->record(0);
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

void utilities::linkQComboBoxAndModel(QComboBox *comboBox, QSqlTableModel *model, QVariant showKey)
{
    comboBox->clear();
    comboBox->setModel(model);
    QListView* qtv = new QListView(comboBox);
    comboBox->setView(qtv);
    //! Show key column only
    comboBox->setModelColumn(model->fieldIndex(showKey.toString()));
    comboBox->setCurrentIndex(0);
}

void utilities::sqlTableModel2Csv(QSqlTableModel *source, QString filename, QString delimiter)
{
    QFile __file(filename);
    if(__file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data(&__file);

        data.setCodec(QTextCodec::codecForName("UTF-8"));
        //! Load header
        for(int i=0;i<source->columnCount();i++)
        {
            QString __header = source->headerData(i,Qt::Horizontal).toString();
           data << QString("%1%2")
                   .arg(__header)
                   .arg(delimiter);
        }
        data << "\n";
        //! Load datas
        for(int i=0;i<source->rowCount();i++)
        {
            data << QString("%1\n").arg(sqlRecord2DelimitedString(source->record(i)));
        }

        __file.close();
    }
}

QString utilities::sqlRecord2DelimitedString(QSqlRecord record, QString delimiter)
{
    QString __output;
    for(int i=0;i<record.count();i++)
        __output.append(QString("%1%2").arg(record.value(i).toString()).arg(delimiter));
    return __output;
}

QSqlTableModel * utilities::duplicate(QSqlTableModel * source)
{
	QSqlTableModel* result = new QSqlTableModel(nullptr, source->database());
	result->setTable(source->tableName());
	return result;
}

QMap<QString,QSqlTableModel*> utilities::m_cachedTables;

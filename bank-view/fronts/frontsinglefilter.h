#ifndef FRONTSINGLEFILTER_H
#define FRONTSINGLEFILTER_H

#include <QWidget>

#include <QSqlTableModel>
#include <QTableView>
#include <QListView>

#include <utilities.h>
#include <definitionauxiliarykeys.h>

//#include <abstractauthreceiver.h>

using namespace HEADER_STRUCTURE;

namespace Ui {
class FrontSingleFilter;
}

class FrontSingleFilter : public QWidget
{
    Q_OBJECT
public:
    explicit FrontSingleFilter(QWidget *parent = nullptr);

    void PrimaryTable(QSqlTableModel* model,QVariant column = QString("zh_TW"));
    void DataTable(QSqlTableModel* model)
    {
        m_dataTable = model;
    }
    void DataKey(QVariant key);
protected slots:
    void onSelectedIndexChanged(int i);
    void onSelectAll();
protected:
     Ui::FrontSingleFilter *ui;

    QSqlTableModel* m_dataTable;
    QVariant m_dataKey;
};

#endif // FRONTSINGLEFILTER_H

#ifndef FRONTSINGLEFILTER_H
#define FRONTSINGLEFILTER_H

#include <QWidget>

#include <QSqlTableModel>
#include <QTableView>
#include <QListView>
namespace Ui {
class FrontSingleFilter;
}

class FrontSingleFilter : public QWidget
{
    Q_OBJECT
public:
    explicit FrontSingleFilter(QSqlTableModel* dataTable,
                               QSqlTableModel* primaryTable,
                               QString key = "REGION",
                               QString showColumn = "NAME",
                               QWidget *parent = nullptr);

signals:

public slots:
protected slots:
    void onSelectedIndexChanged(int i);
    void onSelectAll();
protected:
     Ui::FrontSingleFilter *ui;

    QSqlTableModel* __dataTable;
    QString __key;
};

#endif // FRONTSINGLEFILTER_H

#ifndef FRONTWINFILTER_H
#define FRONTWINFILTER_H

#include <QWidget>
#include <QList>
#include <QTableView>
#include <QListView>
#include <QMap>

#include <abstractsqltableadpater.h>
#include <QSqlTableModel>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontTwinFilter;
}

class FrontTwinFilter : public QWidget,
        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontTwinFilter(QWidget *parent = nullptr);

    explicit FrontTwinFilter(QSqlTableModel* dataTable,
                             QVariant key1,
                             QSqlTableModel* primaryTable,
                             QVariant key2,
                             QSqlTableModel* secondaryTable,
                             QWidget *parent = 0);
    ~FrontTwinFilter();

signals:
    void primarySelected(QVariant primaryKey);
    void secondarySelected(QVariant secondaryKey);
protected slots:
    void onSelectionChanged(int i);
    void onSelectAll();
private:
    Ui::FrontTwinFilter *ui;

    QSqlTableModel* __dataTable;
    QVariant __key1;
    //!
    //! \brief __primaryTable
    //! Load to
    QSqlTableModel* __primaryTable;
    QVariant __key2;
    //!
    //! \brief __secondaryTable
    //!
    QSqlTableModel* __secondaryTable;

    //!
    QMap<QWidget*,QSqlTableModel*> __manipulateMap;
};

#endif // FRONTWINFILTER_H

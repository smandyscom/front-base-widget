#ifndef FRONTWINFILTER_H
#define FRONTWINFILTER_H

#include <QWidget>
#include <QList>
#include <QTableView>
#include <QMap>

#include <abstractqvariantsqltable.h>

namespace Ui {
class FrontTwinFilter;
}

class FrontTwinFilter : public QWidget
{
    Q_OBJECT

public:
//    enum KeyWords
//    {
//        REGION,
//        NAME,
//    }
//    Q_ENUM(KeyWords)

    explicit FrontTwinFilter(AbstractQVariantSqlTable* dataTable,
                             AbstractQVariantSqlTable* primaryTable,
                             AbstractQVariantSqlTable* regionTable,
                             QString primaryKey,
                             QWidget *parent = 0);
    ~FrontTwinFilter();
protected slots:
    void onSelectionChanged(int i);
    void onSelectAll();
private:
    Ui::FrontTwinFilter *ui;

    AbstractQVariantSqlTable* __dataTable;
    QString __primaryKeyName;
    //!
    //! \brief __primaryTable
    //! Load to
    AbstractQVariantSqlTable* __primaryTable;
    AbstractQVariantSqlTable* __regionTable;

    //!
    QMap<QWidget*,AbstractQVariantSqlTable*> __manipulateMap;
    QMap<QWidget*,QString> __fieldNameMap;
};

#endif // FRONTWINFILTER_H

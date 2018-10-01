#ifndef TABLEVIEWFEATURE_H
#define TABLEVIEWFEATURE_H

#include <QTableView>
#include <delegateviewitemfeaturemarker.h>

//!
//! \brief The TableViewFeature class
//! Loaded with Feature marker
class TableViewFeature :
        public QTableView
{
    Q_OBJECT
public:
    explicit TableViewFeature(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // TABLEVIEWFEATURE_H

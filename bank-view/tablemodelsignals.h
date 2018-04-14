#ifndef TABLEMODELSIGNALS_H
#define TABLEMODELSIGNALS_H

#include <QObject>

class TableModelSignals : public QObject
{
    Q_OBJECT
public:
    explicit TableModelSignals(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TABLEMODELSIGNALS_H
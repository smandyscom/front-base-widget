#ifndef BITFLIPITEMDELEGATE_H
#define BITFLIPITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class BitFlipItemDelegate : public QObject
{
    Q_OBJECT
public:
    explicit BitFlipItemDelegate(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BITFLIPITEMDELEGATE_H

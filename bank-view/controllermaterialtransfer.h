#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>

class ControllerMaterialTransfer : public QObject
{
    Q_OBJECT
public:
    explicit ControllerMaterialTransfer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CONTROLLERMATERIALTRANSFER_H
#ifndef FRONTSLOT_H
#define FRONTSLOT_H

#include <QWidget>

#include <controllermaterialtransfer.h>
#include <utilities.h>
#include <definitionauxiliarykeys.h>

#include <QDialog>
#include <QTableView>

namespace Ui {
class FrontSlot;
}

class FrontSlot : public QWidget
{
    Q_OBJECT

public:
    explicit FrontSlot(ControllerMaterialTransfer* controller, QWidget *parent = 0);
    ~FrontSlot();

protected:
    void onDataUpdated();
    void onDataRaise();
private:
    Ui::FrontSlot *ui;

    ControllerMaterialTransfer* __controller;
    QDialog* __dialog;
};

#endif // FRONTSLOT_H
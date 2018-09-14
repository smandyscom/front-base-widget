#ifndef FRONTSLOT_H
#define FRONTSLOT_H

#include <QWidget>

#include <junctionbankdatabase.h>
#include <controllermaterialtransfer.h>
#include <utilities.h>
#include <definitionauxiliarykeys.h>

#include <QDialog>
#include <QTableView>
#include <QFormLayout>

namespace Ui {
class FrontSlot;
}

class FrontSlot : public QWidget
{
    Q_OBJECT

public:
    explicit FrontSlot(ControllerMaterialTransfer* controller, bool isShowCounters=true, QWidget *parent = 0);
    ~FrontSlot();

protected:
    void onDataUpdated();
    void onDataRaise();
    void onMaterialOverrideOff();
    void onClear();
private:
    Ui::FrontSlot *ui;

    ControllerMaterialTransfer* __controller;
    QDialog* __dialog;
    QTableView* __view;
};

#endif // FRONTSLOT_H

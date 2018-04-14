#ifndef FRONTBANKTRANSFER_H
#define FRONTBANKTRANSFER_H

#include <QWidget>
#include <QTimer>
#include <QList>

#include <controllerbanktransfer.h>
#include <definitionbasicblocks.h>

namespace Ui {
class FrontBankTransfer;
}

class FrontBankTransfer : public QWidget
{
    Q_OBJECT

public:
    explicit FrontBankTransfer(CommitBlock::CommitDataBlockSelection selection = CommitBlock::SELECTION_COMMAND_BLOCK,
                               QWidget *parent = 0);
    ~FrontBankTransfer();
protected slots:
    void onButtonClicked();
    void onScanning();
private:
    Ui::FrontBankTransfer *ui;
    QTimer* __timer;
    CommitBlock::CommitDataBlockSelection __selection;

    QList<QWidget*> __interlockedList;

    ControllerBankTransfer* __transfer;


};

#endif // FRONTBANKTRANSFER_H

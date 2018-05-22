#ifndef FRONTBANKTRANSFER_H
#define FRONTBANKTRANSFER_H

#include <QWidget>
#include <QTimer>
#include <QList>

#include <controllerbanktransfer.h>
#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontBankTransfer;
}

using namespace HEADER_STRUCTURE;

//!
//! \brief The FrontBankTransfer class
//! Transfer all mode
class FrontBankTransfer : public QWidget,
        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontBankTransfer(QWidget *parent = 0);
    ~FrontBankTransfer();

    void Selection(CommitBlock::CommitDataBlockSelection value);

    ControllerBankTransfer* ControllerTransfer() const
    {
        return __controllerTransfer;
    }

    static QSqlTableModel* ObjectTable;

protected slots:
    void onButtonClicked();
    void onScanning();
    void onTransfering(TransferTask task);
private:
    Ui::FrontBankTransfer *ui;
    QTimer* __timer;
    CommitBlock::CommitDataBlockSelection __selection;

    QList<QWidget*> __interlockedList;
    QString __titlePrefix;

    ControllerBankTransfer* __controllerTransfer;
};

#endif // FRONTBANKTRANSFER_H

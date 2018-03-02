#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <controllermanualmode.h>
#include <QAbstractTableModel>
#include <junctionbankdatabase.h>
//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer : public QObject
{
    Q_OBJECT
public:
    explicit ControllerBankTransfer(QObject *parent = nullptr);

    void GoalCount(int value) {__goalCount = value;}
    int GoalCount() {return __goalCount;}
public slots:
    //!
    //! \brief onUpdatedFromPLC
    //! Fetching commandblocks from PLC , stored in SqlTabeModel(From index 0-N
    void onUpdatedFromPlc();
    //!
    //! \brief onCommitToPlc
    //! Commiting command blocks to PLC (From index 0-N
    void onCommitToPlc();
protected slots:
    void onOperationPerformed();
protected:
    int __currentIndex;
    int __goalCount;
    ExtendedCommandBlock __commandBlock;
    CommitBlock __commitOption;
    ControllerManualMode* __controller;

    QAbstractTableModel* __commandBankTable;
};

#endif // CONTROLLERBANKMANAGER_H

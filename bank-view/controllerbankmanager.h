#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <controllermanualmode.h>
#include <QAbstractTableModel>
#include <junctionbankdatabase.h>
//!
//! \brief The ControllerBankManager class
//! Offered the UI of SQL VIEW , which combined by two table (Name tags and Values
class ControllerBankManager : public QObject
{
    Q_OBJECT
public:
    explicit ControllerBankManager(QObject *parent = nullptr);
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
private:
    int currentIndex;
    ExtendedCommandBlock genericCommandBlock;
    ControllerManualMode* controller;

    QAbstractTableModel* __commandBankTable;
};

#endif // CONTROLLERBANKMANAGER_H

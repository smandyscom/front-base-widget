#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <controllermanualmode.h>
#include <QAbstractTableModel>
#include <tablemodelcommandblock.h>
//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer : public QObject
{
    Q_OBJECT
public:    
    explicit ControllerBankTransfer(TableModelCommandBlock* model,QObject *parent = nullptr);

    int Goal() const {return __goal;}
    int CurrentIndex() const {return __currentIndex;}
    bool IsProcessing() const { return __currentIndex != -1;}
    qreal Progress() const { return (__currentIndex+1)/__goal;}

public slots:
    //!
    //! \brief onTransferData
    //! \param mode
    //! \param rowIndex non -1, single mode, -1 ,batch mode
    //!
    void onTransferData(CommitBlock::CommitMode mode,int rowIndex = -1);
protected slots:
    void onOperationPerformed();
protected:
    int __currentIndex;
    int __goal;

    CommitBlock __commitOption;

    TableModelCommandBlock* __model;
    //!
    //! \brief __controller
    //! From the sigalton
    ControllerManualMode* __controller;

};

#endif // CONTROLLERBANKMANAGER_H

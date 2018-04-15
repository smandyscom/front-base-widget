#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>

#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

#include <QMap>

#include <controllermanualmode.h>
#include <abstractsqltableadpater.h>

//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer : public QObject
{
    Q_OBJECT
public:
    enum ModeAndStatus
    {
        BATCH_MODE=-1,
        PROCESSING=-1,
    };
    Q_ENUM(ModeAndStatus)



    int Goal() const {return __goal;}
    int CurrentIndex() const {return __currentIndex;}
    bool IsProcessing() const {
        return __currentIndex == PROCESSING ||
                __controller->CurrentState()!=ControllerManualMode::STATE_IDLE;
    }
    qreal Progress() const { return (__currentIndex+1)/__goal;}

    void Adaptor(CommitBlock::CommitDataBlockSelection key,AbstractSqlTableAdpater* value)
    {
        __adaptorMap[key] = value;
    }
    void DataBlockSelection(CommitBlock::CommitDataBlockSelection value)
    {
        __adaptor = __adaptorMap[value];
        __commitOption.Selection(value);
    }
    CommitBlock::CommitDataBlockSelection DataBlockSelection() const
    {
        return __commitOption.Selection();
    }
    void Direction(CommitBlock::CommitMode value)
    {
        __commitOption.Mode(value);
    }

    static ControllerBankTransfer* Instance();

signals:
    void dataTransfered();
    void dataTransfering();
public slots:
    //!
    //! \brief onTransferData
    //! \param mode
    //! \param rowIndex non -1, single mode, -1 ,batch mode
    //!
    void onTransferData(int rowIndex = BATCH_MODE);
protected slots:
    void onControllerOperationPerformed();
protected:
    explicit ControllerBankTransfer(QObject *parent = nullptr);

    int __currentIndex;
    int __goal;

    CommitBlock __commitOption;

    QMap<CommitBlock::CommitDataBlockSelection,AbstractSqlTableAdpater*> __adaptorMap;

    AbstractSqlTableAdpater* __adaptor;
    //!
    //! \brief __controller
    //! From the sigalton
    ControllerManualMode* __controller;

    void transfer();

    static ControllerBankTransfer* __instance;
};

#endif // CONTROLLERBANKMANAGER_H

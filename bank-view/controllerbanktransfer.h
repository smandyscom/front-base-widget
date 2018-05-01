#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>

#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

#include <QMap>

#include <controllermanualmode.h>
#include <abstractsqltableadpater.h>

typedef QPair<CommitBlock::CommitDataBlockSelection,int> TransferTask;
//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer : public QObject
{
    Q_OBJECT
public:
    enum ModeAndStatus
    {
        //! Mode
        BATCH_ALL_MODE=-1,
        BATCH_PRESCHEDUALED_MODE=-2,
        //! Status
        PROCESSING=-1,
    };
    Q_ENUM(ModeAndStatus)

    //! Status
    int RestTasksCount() const {return __tasksQueue.count();}
    bool IsProcessing() const {
        return !__tasksQueue.isEmpty() ||
                __controller->CurrentState()!=ControllerManualMode::STATE_IDLE;
    }
    //! Linkage and configuration
    static void Adaptor(CommitBlock::CommitDataBlockSelection key,AbstractSqlTableAdpater* value)
    {
        __adaptorMap[key] = value;
    }
    static AbstractSqlTableAdpater* Adaptor(CommitBlock::CommitDataBlockSelection key)
    {
        return __adaptorMap[key];
    }

    void Direction(CommitBlock::CommitMode value)
    {
        __commitOption.Mode(value);
    }

    void PutTask(TransferTask task);


    explicit ControllerBankTransfer(QObject *parent = nullptr);

signals:
    void dataTransfered();
    void dataTransfering(TransferTask task);
public slots:
    //!
    //! \brief onTransferData
    //! \param mode
    //! \param rowIndex non -1, single mode, -1 ,batch mode
    //!
    void onTransferData();
protected slots:
    void onControllerOperationPerformed();
protected:

    CommitBlock __commitOption;

    static QMap<CommitBlock::CommitDataBlockSelection,AbstractSqlTableAdpater*> __adaptorMap;

    AbstractSqlTableAdpater* __adaptor;
    //!
    //! \brief __controller
    //! From the sigalton
    ControllerManualMode* __controller;

    void transfer();
    QQueue<TransferTask> __tasksQueue;

};

#endif // CONTROLLERBANKMANAGER_H

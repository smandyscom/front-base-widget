#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <QMap>

#include <controllermanualmode.h>
#include <abstractsqltableadpater.h>

//！ Categrory and record index
typedef QPair<ManualModeDataBlock::Categrories,int> TransferTask;
//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer :
        public ControllerManualMode
{
    Q_OBJECT
public:
    enum ModeAndStatus
    {
        //! Mode
        //! Transmit all record in categrory
        BATCH_ALL_MODE=-1,
        //! Transmit pre-schdualed records (index
        BATCH_PRESCHEDUALED_MODE=-2,
        //! Status
        PROCESSING=-1,
    };
    Q_ENUM(ModeAndStatus)

    //! Status
//    int RestTasksCount() const {return __tasksQueue.count();}
//    bool IsProcessing() const {
//        return !__tasksQueue.isEmpty() ||
//                m_controller->CurrentState()!=ControllerManualMode::STATE_IDLE;
//    }
    //! Linkage and configuration
    void Adaptor(ManualModeDataBlock::Categrories key,AbstractSqlTableAdpater* value)
    {
        m_adaptors[key] = value;
        //! Sense data changed and put task
        connect(value->Model(),&QSqlTableModel::dataChanged,this,&ControllerBankTransfer::onDataChanged);
    }
//    AbstractSqlTableAdpater* Adaptor(ManualModeDataBlock::Categrories key)
//    {
//        return m_adaptors[key];
//    }

//    void Direction(CommitBlock::CommitMode value)
//    {
//        __commitOption.Mode(value);
//    }

    void PutTask(TransferTask task);


    explicit ControllerBankTransfer(QObject *parent = nullptr);

signals:
//    void dataTransfered();
//    void dataTransfering(TransferTask task);
public slots:
    //!
    //! \brief onTransferData
    //! \param mode
    //! \param rowIndex non -1, single mode, -1 ,batch mode
    //!
//    void onTransferData();
    void onDataChanged();
protected slots:
    //!
    //! \brief onOperationTrigger
    //! Internal loop link
//    void onOperationTrigger();
//    void onOperationReady();
protected:
    ManualModeDataBlock::Mode m_mode;
    //! By task queue to write-in
    ManualModeDataBlock::Categrories m_categrory;
    int m_index;
    QMap<ManualModeDataBlock::Categrories,AbstractSqlTableAdpater*> m_adaptors;

    AbstractSqlTableAdpater* m_current_adaptor;
    //!
    //! \brief __controller
    //! From the sigalton
    QObject* m_controller;

    void transfer();
    QQueue<TransferTask> m_tasksQueue;

    //!
    //! \brief doneOn
    void plcReady() Q_DECL_OVERRIDE;
    void runOn() Q_DECL_OVERRIDE;
    void doneOn() Q_DECL_OVERRIDE;
    void doneOff() Q_DECL_OVERRIDE;
};

#endif // CONTROLLERBANKMANAGER_H

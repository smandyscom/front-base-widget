#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <QMap>

#include <controllerbase.h>
#include <definitionmanualblock.h>

#include <abstractsqltableadpater.h>

//！ Categrory and record index
typedef QPair<ManualModeDataBlock::Categrories,int> TransferTask;
//!
//! \brief The ControllerBankManager class
//! Charge the operation about transfer from/to PLC
class ControllerBankTransfer :
        public ControllerBase
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
    int RestTasksCount() const {return __tasksQueue.count();}
    bool IsProcessing() const {
        return !__tasksQueue.isEmpty() ||
                m_controller->CurrentState()!=ControllerManualMode::STATE_IDLE;
    }
    //! Linkage and configuration
    static void Adaptor(CommitBlock::CommitCategrories key,AbstractSqlTableAdpater* value)
    {
        m_adaptors[key] = value;
    }
    static AbstractSqlTableAdpater* Adaptor(CommitBlock::CommitCategrories key)
    {
        return m_adaptors[key];
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
    void onControllerOperationReady();
protected:

    CommitBlock __commitOption;

    static QMap<ManualModeDataBlock::Categrories,AbstractSqlTableAdpater*> m_adaptors;

    AbstractSqlTableAdpater* m_current_adaptor;
    //!
    //! \brief __controller
    //! From the sigalton
    QObject* m_controller;

    void transfer();
    QQueue<TransferTask> m_tasksQueue;

    //!
    //! \brief m_operator_propertyChanged
    //! \param key
    //! \param value
    //!
    void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERBANKMANAGER_H

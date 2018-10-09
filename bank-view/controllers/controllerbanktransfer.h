#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <QMap>
#include <QQueue>

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

    void Adaptor(ManualModeDataBlock::Categrories key,AbstractSqlTableAdpater* value);

    explicit ControllerBankTransfer(quint8 clientId, quint16 baseOffset, int interval, QObject *parent = nullptr);

signals:
public slots:
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>());
protected slots:
protected:
    ManualModeDataBlock::Mode m_mode();
    //! By task queue to write-in
    ManualModeDataBlock::Categrories m_categrory;
    int m_index;
    QMap<ManualModeDataBlock::Categrories,AbstractSqlTableAdpater*> m_adaptors;

    //!
    //! \brief __controller
    //! From the sigalton
    QObject* m_controller;

    void transfer();
    QQueue<TransferTask> m_tasksQueue;

    //!
    //! \brief doneOn
    void plcReady() Q_DECL_OVERRIDE;
    void doneOn() Q_DECL_OVERRIDE;
    void doneOff() Q_DECL_OVERRIDE;

    //!
    void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERBANKMANAGER_H

#ifndef CONTROLLERIOMONITOR_H
#define CONTROLLERIOMONITOR_H

#include <QObject>
#include <QSqlTableModel>

#include <controllerbase.h>

//!
//! \brief The ControllerIOMonitor class
//! Monitoring specific client
class ControllerIOMonitor :
        public ControllerBase
{
    Q_OBJECT
public:
    explicit ControllerIOMonitor(int interval, QObject *parent = nullptr);

    void AttachReceiver(QObject* receiver) Q_DECL_OVERRIDE;

    void AppendPair(QVariant address,QVariant name);
signals:

public slots:

protected slots:
    //!
    //! \brief onAcknowledged
    //! \param ack
    //! Routine
    void onAcknowledged(InterfaceRequest ack) Q_DECL_OVERRIDE;
    //!
    //! \brief onDataChanged
    //! \param topLeft
    //! \param bottomRight
    //! \param roles
    //! Output override
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
protected:
    //!
    //! \brief m_model
    //! First receiver
    QSqlTableModel* m_model;
    //!
    //! \brief m_addressNameTable
    //! pair of Address,Name
    QMap<QVariant,QVariant> m_addressNameTable;
};

#endif // CONTROLLERIOMONITOR_H

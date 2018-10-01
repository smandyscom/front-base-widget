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
    explicit ControllerIOMonitor(quint8 clientId, int interval, QObject *parent = nullptr);

signals:

public slots:

protected slots:
    //!
    //! \brief onAcknowledged
    //! \param ack
    //! Routine
    void onAcknowledged(InterfaceRequest ack) Q_DECL_OVERRIDE;

protected:

    QSqlTableModel* m_model;
    //!
    //! \brief m_addressNameTable
    //! pair of Address,Name
    QMap<QVariant,QVariant> m_addressNameTable;
    //! TODO , Static? Or Direct use channel's routine to check
    QList<ADDRESS_MODE> m_baseAddressTable;

    int m_columnIndexAddress;
    int m_columnIndexName;
};

#endif // CONTROLLERIOMONITOR_H

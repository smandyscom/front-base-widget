#ifndef CONTROLLERIOMONITOR_H
#define CONTROLLERIOMONITOR_H

#include <QObject>
#include <QSqlTableModel>

#include <controllerbase.h>

//!
//! \brief The ControllerIOMonitor class
//! Monitoring specific model
//! Dedicated client
class ControllerIOMonitor :
        public ControllerBase
{
    Q_OBJECT
public:
    enum Mode
    {
        POLLING,
        //! No polling , handling model only
        NO_POLLING,
    };

    explicit ControllerIOMonitor(quint8 clientId,Mode mode=POLLING, QObject *parent = nullptr);

    void setModel(QSqlTableModel *model,
                  QMap<QVariant,QVariant> addressNameMap);

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

	void onModelReset();

protected:
    Mode m_mode;
    //! Store address and modelIndex relation(Name
    QList<ADDRESS_MODE> m_addressList;
    QMap<ADDRESS_MODE,QVariant> m_addressIndexTable;
    void findAndUpdate(ADDRESS_MODE address);

    //!
    //! \brief m_model
    //! First receiver
    QSqlTableModel* m_model;
    
	QMap<QVariant, QVariant> m_addressNameMap;
};

#endif // CONTROLLERIOMONITOR_H

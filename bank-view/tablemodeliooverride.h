#ifndef TABLEMODELIOOVERRIDE_H
#define TABLEMODELIOOVERRIDE_H

#include <QObject>
#include <modbuschannel.h>
#include <QColor>
#include <QTimer>
#include <QSqlRelationalTableModel>
//!
//! \brief The TableModelIOOverride class
//! Override/Read the I/O data
class TableModelIOOverride : public QSqlTableModel
{
    Q_OBJECT
public:
    enum Header
    {
        ID,
        HAL_ADDRESS,
        NAME,
        REGION,
        PLC_ADDRESS,
        PHYSICAL_ADDRESS,
        TERMINAL
    };
    Q_ENUM(Header)
    explicit TableModelIOOverride(QSqlTableModel* source,Qt::GlobalColor trueColor=Qt::green);

    //!
    //! \brief data
    //! \param idx
    //! \param role
    //! \return
    //! Override the Background role
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

protected:
    ModbusChannel* __channel;
    ModbusDriverAddress CurrentIndexAddress(int rowIndex) const;

    Qt::GlobalColor __trueColor;
    QTimer* __timer;
};

#endif // TABLEMODELIOOVERRIDE_H

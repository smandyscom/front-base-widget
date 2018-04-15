#ifndef TABLEMODELIOOVERRIDE_H
#define TABLEMODELIOOVERRIDE_H

#include <QObject>
#include <modbuschannel.h>
#include <QColor>
#include <QTimer>
#include <abstractsqltableadpater.h>
//!
//! \brief The TableModelIOOverride class
//! Override/Read the I/O data
class TableModelIOOverride : public AbstractQVariantSqlTable
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
    explicit TableModelIOOverride(QObject *parent = nullptr);

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

    QTimer* __timer;
};

#endif // TABLEMODELIOOVERRIDE_H

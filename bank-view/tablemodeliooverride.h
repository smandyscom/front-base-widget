#ifndef TABLEMODELIOOVERRIDE_H
#define TABLEMODELIOOVERRIDE_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <modbuschannel.h>
#include <QColor>
//!
//! \brief The TableModelIOOverride class
//! Override/Read the I/O data
class TableModelIOOverride : public QSqlRelationalTableModel
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

    void IsEnableStateControl(bool value);
    bool IsEnableStateControl(void) const;


    //!
    //! \brief data
    //! \param idx
    //! \param role
    //! \return
    //! Override the Background role
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    //!
    //! \brief setData
    //! \param index
    //! \param value
    //! \param role
    //! Offer the ability to control STATE
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

protected:
    bool __isEnableStateControl;
    ModbusChannel* __channel;
    ModbusDriverAddress CurrentIndexAddress(int rowIndex) const;
};

#endif // TABLEMODELIOOVERRIDE_H

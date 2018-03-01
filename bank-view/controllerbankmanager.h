#ifndef CONTROLLERBANKMANAGER_H
#define CONTROLLERBANKMANAGER_H

#include <QObject>
#include <controllermanualmode.h>
#include <QAbstractTableModel>

//!
//! \brief The ControllerBankManager class
//! Offered the UI of SQL VIEW , which combined by two table (Name tags and Values
class ControllerBankManager : public QAbstractTableModel
{
    enum FieldHeaders
    {
        HEADER_OBJECT_ID=0,
        HEADER_COMMAND_TYPE=1,
        HEADER_SPEED=2,
        HEADER_ACCERALATION=3,
        HEADER_DECERALATION=4,
        HEADER_TORQUE_LIMIT=5,
        HEADER_EXTEND_CONTROL_WORD=6,
        HEADER_EXT1_COORD1_OFFSET=7,
        HEADER_EXT2_COORD2_SPEED_CREEP=8,
        HEADER_EXT3_COORD3_SPEED_APPROACH=9
    };

    Q_OBJECT
public:
    explicit ControllerBankManager(QObject *parent = nullptr);
public slots:
    //!
    //! \brief onUpdatedFromPLC
    //! Fetching commandblocks from PLC , stored in SqlTabeModel(From index 0-N
    void onUpdatedFromPlc();
    //!
    //! \brief onCommitToPlc
    //! Commiting command blocks to PLC (From index 0-N
    void onCommitToPlc();
protected slots:
    void onOperationPerformed();
private:
    int currentIndex;
    ExtendedCommandBlock genericCommandBlock;
    ControllerManualMode* controller;
};

#endif // CONTROLLERBANKMANAGER_H

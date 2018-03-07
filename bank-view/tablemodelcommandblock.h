#ifndef TABLEMODELCOMMANDBLOCK_H
#define TABLEMODELCOMMANDBLOCK_H

#include <QObject>
#include <QSqlTableModel>
#include <basicblocksdefinition.h>
//!
//! \brief The TableModelCommandBlock class
//! Define how to convert row to/from ExtendCommandBlock
//! Decoration mode
class TableModelCommandBlock : public QSqlTableModel
{
    Q_OBJECT
public:
    //!
    //! \brief The CommandBlockTableHeaders enum
    //! Fully reflect WHOLE_COMMAND_BLOCKS
    enum Headers
    {
        COMMAND_BLOCK_ID = 0,
        COMMAND_TYPE,
        AXIS_ID,
        NAME,
        EXT_CONTROL_BIT_0,
        COORD1,
        SPEED,
        ACC_TIME,
        DEC_TIME,
        TORQUE_LIMIT,
        COMMENT,
        IS_RESET_POS_R,
        RESERVED_WORD,
        COORD2,
        COORD3,
    };
    Q_ENUM(Headers)

    explicit TableModelCommandBlock(QSqlTableModel* instance);

    ExtendedCommandBlock Row(int rowIndex);
    void Row(int rowIndex, const ExtendedCommandBlock value);
protected:
    QSqlTableModel* __instance;
};

#endif // TABLEMODELCOMMANDBLOCK_H
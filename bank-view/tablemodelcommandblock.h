#ifndef TABLEMODELCOMMANDBLOCK_H
#define TABLEMODELCOMMANDBLOCK_H

#include <QObject>
#include <basicblocksdefinition.h>
#include <abstractqvariantsqltable.h>

#include <QSqlRecord>
//!
//! \brief The TableModelCommandBlock class
//! Define how to convert row to/from ExtendCommandBlock
//! Decoration mode
class TableModelCommandBlock : public AbstractQVariantSqlTable
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
        EXT_CTRL_BIT_0,
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

    explicit TableModelCommandBlock(QObject *parent = Q_NULLPTR);

    QVariant RowRecord(int rowIndex) const Q_DECL_OVERRIDE;
    void RowRecord(int rowIndex, const QVariant value) Q_DECL_OVERRIDE;

};

#endif // TABLEMODELCOMMANDBLOCK_H

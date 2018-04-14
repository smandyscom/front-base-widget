#ifndef TABLEMODELCOMMANDBLOCK_H
#define TABLEMODELCOMMANDBLOCK_H

#include <QObject>
#include <definitioncommandblock.h>
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
        COMMAND_BLOCK_ID,
        NAME,
        COMMENT,
        //! Data
        COMMAND_TYPE = AbstractCommandBlock::OFFSET_ACB_COMMAND_TYPE,
        AXIS_ID = AbstractCommandBlock::OFFSET_ACB_AXIS_ID,
        SPEED = AbstractCommandBlock::OFFSET_ACB_SPD,
        ACC_TIME = AbstractCommandBlock::OFFSET_ACB_ACC_T,
        DEC_TIME = AbstractCommandBlock::OFFSET_ACB_DEC_T,
        TORQUE_LIMIT= AbstractCommandBlock::OFFSET_ACB_TOR_LIMIT,
        //! Data
        EXT_CTRL_BIT_0 = ExtendedCommandBlock::OFFSET_ECB_CONTROL_WORD,
        RESERVED_WORD = ExtendedCommandBlock::OFFSET_ECB_RESERVE_WORD,
        COORD1 = ExtendedCommandBlock::OFFSET_ECB_COORD1,
        COORD2 = ExtendedCommandBlock::OFFSET_ECB_COORD2,
        COORD3 = ExtendedCommandBlock::OFFSET_ECB_COORD3,
    };
    Q_ENUM(Headers)

    explicit TableModelCommandBlock(AbstractQVariantSqlTable *decorator):
        AbstractQVariantSqlTable(decorator)
    {
        __block = new AbstractCommandBlock();

        __wordHeaders = {
            QVariant::fromValue(COMMAND_TYPE),
            QVariant::fromValue(AXIS_ID),
            QVariant::fromValue(EXT_CTRL_BIT_0),
            QVariant::fromValue(RESERVED_WORD),
        };
        __qrealHeaders = {
            QVariant::fromValue(SPEED),
            QVariant::fromValue(ACC_TIME),
            QVariant::fromValue(DEC_TIME),
            QVariant::fromValue(TORQUE_LIMIT),
            QVariant::fromValue(COORD1),
            QVariant::fromValue(COORD2),
            QVariant::fromValue(COORD3),
        };
    }

    QVariant RowRecord(int rowIndex) const Q_DECL_OVERRIDE;
    void RowRecord(int rowIndex,QVariant value) Q_DECL_OVERRIDE;
};

#endif // TABLEMODELCOMMANDBLOCK_H

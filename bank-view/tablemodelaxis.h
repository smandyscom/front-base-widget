#ifndef TABLEMODELAXIS_H
#define TABLEMODELAXIS_H

#include <QObject>
#include <abstractqvariantsqltable.h>
#include <definitionaxisblocks.h>

//!
//! \brief The TableModelAxis class
//! Decoration mode
class TableModelAxis : public AbstractQVariantSqlTable
{
    Q_OBJECT
public:
    //!
    //! \brief The Headers enum
    //! Intrested headers
    enum Headers
    {
        AXIS_ID,
        REGION,
        NAME,
        COMMENT,
        //! Data
        ADDRESS = AxisContextBlock::OFFSET_CONTEXT_ADDRESS,
        TYPE = AxisContextBlock::OFFSET_CONTEXT_AXIS_TYPE,
        LIMIT_MINUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_MINUS,
        LIMIT_PLUS = AxisContextBlock::OFFSET_CONTEXT_LIMIT_PLUS,
        POSITION_TOLERANCE = AxisContextBlock::OFFSET_CONTEXT_POS_TOLERANCE,
        SPEED_MAX = AxisContextBlock::OFFSET_CONTEXT_SPEED_MAX,
    };
    Q_ENUM(Headers)
    explicit TableModelAxis(AbstractQVariantSqlTable* decorator):
        AbstractQVariantSqlTable(decorator)
    {
        __block = new AxisContextBlock();

        __qrealHeaders = {
            QVariant::fromValue(LIMIT_MINUS),
            QVariant::fromValue(LIMIT_PLUS),
            QVariant::fromValue(POSITION_TOLERANCE),
            QVariant::fromValue(SPEED_MAX)
        };
        __wordHeaders = {
            QVariant::fromValue(ADDRESS),
            QVariant::fromValue(TYPE)
        };
    }
};

#endif // TABLEMODELAXIS_H

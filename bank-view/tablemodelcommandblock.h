#ifndef TABLEMODELCOMMANDBLOCK_H
#define TABLEMODELCOMMANDBLOCK_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <basicblocksdefinition.h>
//!
//! \brief The TableModelCommandBlock class
//! Define how to convert row to/from ExtendCommandBlock
class TableModelCommandBlock : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    enum CommandBlockTableHeaders
    {
        CBT_AXIS_ID,
        CBT_BANK_ID,
        CBT_NAME,
        CBT_IS_ABS,
        CBT_COORDIANTE,
        CBT_SPEED,
        CBT_ACCERALATION,
        CBT_DCCERALATION,
        CBT_TORQUE_LIMIT,
        CBT_COMMENT,
    };
    Q_ENUM(CommandBlockTableHeaders)

    explicit TableModelCommandBlock(QObject *parent = Q_NULLPTR,
                                    QSqlDatabase db = QSqlDatabase());

    ExtendedCommandBlock Row(int rowIndex);
    void Row(int rowIndex,ExtendedCommandBlock value);
};

#endif // TABLEMODELCOMMANDBLOCK_H

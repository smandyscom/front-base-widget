#ifndef TABLEMODELCOMMANDBLOCK_H
#define TABLEMODELCOMMANDBLOCK_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <basicblocksdefinition.h>
//!
//! \brief The TableModelCommandBlock class
//! Define how to convert to/from ExtendCommandBlock
class TableModelCommandBlock : public QSqlRelationalTableModel
{
public:
    TableModelCommandBlock();
};

#endif // TABLEMODELCOMMANDBLOCK_H

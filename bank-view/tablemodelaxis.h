#ifndef TABLEMODELAXIS_H
#define TABLEMODELAXIS_H

#include <QObject>
#include <QSqlRelationalTableModel>
//!
//! \brief The TableModelAxis class
//! Decoration mode
class TableModelAxis : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    enum Headers
    {
        AXIS_ID,
        REGION,
        NAME,
        ADDRESS,
        TYPE,
        LIMIT_MINUS,
        LIMIT_PLUS,
        POSITION_TOLERANCE,
        SPEED_MAX,
        COMMENT,
    };
    Q_ENUM(Headers)
    explicit TableModelAxis(QSqlRelationalTableModel* instance);

    QVariant Value(int id,Headers header) const ;
protected:
    QSqlTableModel* __instance;
};

#endif // TABLEMODELAXIS_H

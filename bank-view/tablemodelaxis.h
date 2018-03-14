#ifndef TABLEMODELAXIS_H
#define TABLEMODELAXIS_H

#include <QObject>
#include <QSqlTableModel>
//!
//! \brief The TableModelAxis class
//! Decoration mode
class TableModelAxis : public QSqlTableModel
{
    Q_OBJECT
public:
    enum Headers
    {
        ID,
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
    explicit TableModelAxis(QSqlTableModel* instance);

    QVariant Value(int id,Headers header) const ;
protected:
    QSqlTableModel* __instance;
};

#endif // TABLEMODELAXIS_H

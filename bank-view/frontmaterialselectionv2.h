#ifndef FRONTMATERIALSELECTIONV2_H
#define FRONTMATERIALSELECTIONV2_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMap>

#include <utilities.h>
#include <definitionslotblock.h>
#include <delegatematerialselector.h>
#include <delegatedataselector.h>

namespace Ui {
class FrontMaterialSelectionV2;
}

class FrontMaterialSelectionV2 : public QWidget
{
    Q_OBJECT

public:
    enum TableNames
    {
        REPORT_HEAD,
        VIEW_MAT_TABLE_KEY,
        VIEW_MAT_TABLE_HOUSING,
        HEADER_REPORT_HEAD
    };
    enum TableFields
    {
        ID,
        en_US,
        zh_TW,
        VALUE,
    };
    enum TableRows
    {
        L_KEY,
        L_HOUSING,
        R_KEY,
        R_HOUSING
    };
    Q_ENUM(TableNames)
    Q_ENUM(TableFields)
    Q_ENUM(TableRows)

    explicit FrontMaterialSelectionV2(QSqlDatabase db,QWidget *parent = 0);
    ~FrontMaterialSelectionV2();

private:
    Ui::FrontMaterialSelectionV2 *ui;
};

#endif // FRONTMATERIALSELECTIONV2_H
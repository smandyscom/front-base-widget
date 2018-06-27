#ifndef FRONTMATERIALSELECTION_H
#define FRONTMATERIALSELECTION_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMap>

#include <utilities.h>


namespace Ui {
class frontMaterialSelection;
}

class frontMaterialSelection : public QWidget
{
    Q_OBJECT
public:
    enum DataFields
    {
        FIELD_L_KEY,
        FIELD_L_HOUSING,
        FIELD_R_KEY,
        FIELD_R_HOUSING,
        FIELD_WORKING_NUMBER
    };
    enum TableNames
    {
        VIEW_MAT_TABLE_KEY,
        VIEW_MAT_TABLE_HOUSING,
    };
    enum TableFields
    {
        ID,
        Polarization,
        POL_H,
        Color,
    };
    Q_ENUM(DataFields)
    Q_ENUM(TableNames)
    Q_ENUM(TableFields)

    explicit frontMaterialSelection(QSqlDatabase db, QWidget *parent = 0);
    ~frontMaterialSelection();
signals:
    void fieldValueChanged(DataFields field,QVariant value);
protected slots:
    void onComboBoxActivated(int index);
    void onWorkingNumberInputed();
private:
    Ui::frontMaterialSelection *ui;

protected:
    QMap<DataFields,QSqlTableModel*> __tableMap;
    QMap<QWidget*,DataFields> __fieldsMap;
};

#endif // FRONTMATERIALSELECTION_H

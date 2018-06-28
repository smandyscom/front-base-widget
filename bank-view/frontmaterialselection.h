#ifndef FRONTMATERIALSELECTION_H
#define FRONTMATERIALSELECTION_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMap>

#include <utilities.h>
#include <definitionslotblock.h>

namespace Ui {
class frontMaterialSelection;
}

class frontMaterialSelection : public QWidget
{
    Q_OBJECT
public:
    enum DataFields : int
    {
        FIELD_L_KEY = SlotBlock::DATA_1,
        FIELD_L_HOUSING = SlotBlock::DATA_2,
        FIELD_R_KEY = SlotBlock::DATA_3,
        FIELD_R_HOUSING = SlotBlock::DATA_4,
        FIELD_WORKING_NUMBER = SlotBlock::DATA_5
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
    void fieldValueChanged(int field,QVariant value);
public slots:
    void onInitialized();
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

#ifndef FRONTCONFIGURATIONTRANSFER_H
#define FRONTCONFIGURATIONTRANSFER_H

#include <QWidget>

#include <frontcommonmanual.h>

#include <definitioncommandblock.h>
#include <definitioncylinderblock.h>
#include <definitionaxisblocks.h>
#include <definitionsignalblock.h>
#include <definitionunitblocks.h>

#include <definitionauxiliarykeys.h>

namespace Ui {
class FrontConfigurationTransfer;
}


using namespace HEADER_STRUCTURE;

class FrontConfigurationTransfer :
        public FrontCommonManual
{
    Q_OBJECT

public:
    explicit FrontConfigurationTransfer(QWidget *parent = 0);
    ~FrontConfigurationTransfer();

    void Setup(QSqlTableModel* commandTable,
               QSqlTableModel* axisTable,
               QSqlTableModel* cylinderTable,
               QSqlTableModel* unitTable,
               QSqlTableModel* signalTable,
               QSqlTableModel* objectTable);

protected slots:
    void onObjectTypeSelected(int rowIndex);

    void onPushButtonClicked();
private:
    Ui::FrontConfigurationTransfer *ui;

    int m_idColumn;

    ManualModeDataBlock::Categrories m_currentObject;
    QMap<ManualModeDataBlock::Categrories,QSqlTableModel*> m_tables;
};

#endif // FRONTCONFIGURATIONTRANSFER_H

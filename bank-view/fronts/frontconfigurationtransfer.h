#ifndef FRONTCONFIGURATIONTRANSFER_H
#define FRONTCONFIGURATIONTRANSFER_H

#include <QWidget>

#include <frontcommonmanual.h>
//#include <frontbanktransfer.h>
//#include <frontsinglefilter.h>

//#include <junctionbankdatabase.h>
#include <definitioncommandblock.h>
#include <definitioncylinderblock.h>
#include <definitionaxisblocks.h>
#include <definitionsignalblock.h>
#include <definitionunitblocks.h>

#include <definitionauxiliarykeys.h>

//#include <controllerbanktransfer.h>

//#include <abstractauthreceiver.h>

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
               QSqlTableModel* objectTable,
               QSqlTableModel* regionTable);

protected slots:
    void onTransferAll();
    void onObjectTypeSelected();
private:
    Ui::FrontConfigurationTransfer *ui;

    QSqlTableModel* m_objectTable;
    QSqlTableModel* m_regionTable;

    QSqlTableModel* m_commandTable;
    QSqlTableModel* m_axisTable;
    QSqlTableModel* m_cylinderTable;
    QSqlTableModel* m_unitTable;
    QSqlTableModel* m_signalTable;

};

#endif // FRONTCONFIGURATIONTRANSFER_H

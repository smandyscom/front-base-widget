#ifndef FRONTCONFIGURATIONTRANSFER_H
#define FRONTCONFIGURATIONTRANSFER_H

#include <QWidget>

#include <frontbanktransfer.h>
#include <frontsinglefilter.h>

#include <junctionbankdatabase.h>
#include <definitionauxiliarykeys.h>

#include <controllerbanktransfer.h>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontConfigurationTransfer;
}


using namespace HEADER_STRUCTURE;

class FrontConfigurationTransfer : public QWidget ,
        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontConfigurationTransfer(QWidget *parent = 0);
    ~FrontConfigurationTransfer();

protected slots:
    void onTransferAll();
    void onObjectTypeSelected();
private:
    Ui::FrontConfigurationTransfer *ui;

    FrontBankTransfer *__fbt;
    FrontSingleFilter *__fsf;

    ControllerBankTransfer* __controllerTransfer;

    QSqlTableModel* __objectTable;

    QSqlTableModel* __regionTable;
    QSqlTableModel* __axisTable;
};

#endif // FRONTCONFIGURATIONTRANSFER_H

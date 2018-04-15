#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <definitionbasicblocks.h>
#include <definitionaxisblocks.h>
#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>
#include <QListView>
#include <QSqlTableModel>

#include <junctionbankdatabase.h>
#include <controllerbanktransfer.h>

#include <abstractsqltableadpater.h>

#include <definitioncommandblock.h>
#include <definitionaxisblocks.h>

namespace Ui {
class FrontManaualMode;
}

using namespace CommandBlock;
using namespace AxisBlock;

//!
//! \brief The FrontManaualMode class
//!
class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QSqlRelationalTableModel *wholeCommandBankModel,
                              QSqlRelationalTableModel *wholeAxisBankModel,
                              QSqlRelationalTableModel *regionModel,
                              QWidget *parent = 0);
    ~FrontManaualMode();
protected slots:
    //!
    //! \brief onButtonBankSetClick
    //! Perform bank set related operations
    void onBankOperationClicked();
    //!
    //! \brief onOperationPerform
    //!
    void onManualOperationClicked();
    //!
    //! \brief onTimerTimeout
    //! Polling monitor status
    void onTimerTimeout();

    void onComboBoxIndexChanged();
    //!
    //! \brief onSubmitted
    //!
    void onDataTransfer();

    void onOperationPerformed();

    void onSelectReset();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    ExtendedCommandBlock __commandBlock;
    CommitBlock __commitOption;
    ControllerManualMode* __controller;

    void setCommonParameters();


    Ui::FrontManaualMode *ui;
    QTimer* __timer;

    int SelectedRowIndex() const;
    MODBUS_WORD SelectedAxisAddress() const;
    MODBUS_WORD SelectedAxisId() const;
    QVariant SelectedAxisValue(TableModelAxis::Headers header) const;

    QSqlRelationalTableModel* __commandBlockTable;
    QSqlRelationalTableModel* __axisTable;
    QSqlRelationalTableModel* __regionTable;

    ControllerBankTransfer* __bankTransfer;


private slots:
};

#endif // FRONTMANAUALMODE_H

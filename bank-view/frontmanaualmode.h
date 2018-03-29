#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <basicblocksdefinition.h>
#include <axisblocksdefinition.h>
#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>
#include <QListView>
#include <QSqlTableModel>

#include <junctionbankdatabase.h>
#include <tablemodelcommandblock.h>
#include <tablemodelaxis.h>
#include <controllerbanktransfer.h>

#include <abstractqvariantsqltable.h>

namespace Ui {
class FrontManaualMode;
}

//!
//! \brief The FrontManaualMode class
//!
class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(AbstractQVariantSqlTable *wholeCommandBankModel,
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

    TableModelCommandBlock* __commandBlockTable;
    TableModelAxis* __axisTable;
    QSqlRelationalTableModel* __regionTable;

    ControllerBankTransfer* __bankTransfer;


private slots:
};

#endif // FRONTMANAUALMODE_H

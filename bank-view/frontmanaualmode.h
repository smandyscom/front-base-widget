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
#include <definitionauxiliarykeys.h>

#include <frontbanktransfer.h>
#include <fronttwinfilter.h>

#include <QSqlRecord>

namespace Ui {
class FrontManaualMode;
}

using namespace CommandBlock;
using namespace DEF_BASIC_DIMENSION;
//!
//! \brief The FrontManaualMode class
//!
class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QWidget *parent = 0);
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

    void onOperationPerformed();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    CommitBlock __commitOption;
    ControllerManualMode* __controller;

    void setCommonParameters(AbstractCommandBlock &__commandBlock);

    Ui::FrontManaualMode *ui;
    QTimer* __timer;

    int SelectedBlockIndex() const;
    QVariant SelectedAxisValue(QVariant keyName) const;
    MODBUS_U_WORD __selectedAxisId;

    //! Front table , fully translated
    QSqlRelationalTableModel* __commandBlockTable;
    QSqlRelationalTableModel* __axisTable;
    QSqlRelationalTableModel* __regionTable;

    AbstractSqlTableAdpater* __commandBlockAdaptor;

    QMap<QWidget*,AxisMonitorBlock::OffsetMonitor> __browserMap;

    QList<QWidget*> __hasSelectionInterlock;
    QList<QWidget*> __busyInterlock;
};

#endif // FRONTMANAUALMODE_H

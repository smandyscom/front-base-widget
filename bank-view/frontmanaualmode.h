#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <basicblocksdefinition.h>

#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>
#include <QListView>
#include <QSqlTableModel>

#include <junctionbankdatabase.h>
#include <tablemodelcommandblock.h>
#include <controllerbanktransfer.h>

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
    explicit FrontManaualMode(QSqlTableModel* wholeCommandBankModel,
                              QSqlTableModel* wholeAxisBankModel,
                              QWidget *parent = 0);
    ~FrontManaualMode();
protected slots:
    //!
    //! \brief onButtonBankSetClick
    //! Perform bank set related operations
    void onBankOperationPerformed();
    //!
    //! \brief onOperationPerform
    //!
    void onOperationPerform();
    //!
    //! \brief onButtonReleased
    //! Feed stop
    void onOperationStopped();

    //!
    //! \brief onFocusChanged
    //! \param old
    //! \param now
    //! Trigger by qApp
    void onFocusChanged(QWidget* old,QWidget* now);
    //!
    //! \brief onTimerTimeout
    //! Polling monitor status
    void onTimerTimeout();

    void onComboBoxIndexChanged();
    //!
    //! \brief onSubmitted
    //!
    void onSubmitted();
    void onTransfer();
protected:
    ExtendedCommandBlock __commandBlock;
    CommitBlock __commitOption;
    ControllerManualMode* __controller;

    void setCommonParameters();


    Ui::FrontManaualMode *ui;
    QTimer* __timer;

    int SelectedRowIndex() const;
    TableModelCommandBlock* __commandBlockTable;
    ControllerBankTransfer* __bankTransfer;


private slots:
};

#endif // FRONTMANAUALMODE_H

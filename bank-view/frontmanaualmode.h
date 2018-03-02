#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <basicblocksdefinition.h>

#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>
#include <QListView>
#include <QAbstractTableModel>

#include <junctionbankdatabase.h>

namespace Ui {
class FrontManaualMode;
}

class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QAbstractTableModel* wholeCommandBankModel,
                              QAbstractTableModel* wholeAxisBankModel,
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
protected:
    ExtendedCommandBlock __commandBlock;
    CommitBlock __commitOption;
    ControllerManualMode* __controller;

    void setCommonParameters();


    Ui::FrontManaualMode *ui;
    QTimer* __timer;

    QModelIndex SelectedRowIndex() const { return ui->tableViewCommandBlock->selectionModel()->selectedRows().first();}
    QAbstractTableModel* CommandBlockTable() const {return ui->tableViewCommandBlock->model();}

};

#endif // FRONTMANAUALMODE_H

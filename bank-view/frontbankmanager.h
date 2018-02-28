#ifndef FRONTBANKMANAGER_H
#define FRONTBANKMANAGER_H

#include <QWidget>
#include <controllermanualmode.h>

namespace Ui {
class FrontBankManager;
}

//!
//! \brief The FrontBankManager class
//! Offered the UI of SQL VIEW , which combined by two table (Name tags and Values
class FrontBankManager : public QWidget
{
    Q_OBJECT

public:
    explicit FrontBankManager(QWidget *parent = 0);
    ~FrontBankManager();
public slots:
    //!
    //! \brief onUpdatedFromPLC
    //! Fetching commandblocks from PLC , stored in SqlTabeModel(From index 0-N
    void onUpdatedFromPlc();
    //!
    //! \brief onCommitToPlc
    //! Commiting command blocks to PLC (From index 0-N
    void onCommitToPlc();

private:
    GenericCommandBlock genericCommandBlock;
    ControllerManualMode* controller;

    Ui::FrontBankManager *ui;
};

#endif // FRONTBANKMANAGER_H

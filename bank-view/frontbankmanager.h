#ifndef FRONTBANKMANAGER_H
#define FRONTBANKMANAGER_H

#include <QWidget>
#include <controllerbankmanager.h>

namespace Ui {
class FrontBankManager;
}

//!
//! \brief The FrontBankManager class
class FrontBankManager : public QWidget
{
    Q_OBJECT

public:
    explicit FrontBankManager(QWidget *parent = 0);
    ~FrontBankManager();
private:
    Ui::FrontBankManager *ui;
};

#endif // FRONTBANKMANAGER_H

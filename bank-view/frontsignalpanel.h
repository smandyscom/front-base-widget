#ifndef FRONTSIGNALPANEL_H
#define FRONTSIGNALPANEL_H

#include <QWidget>

#include <QSqlRelationalTableModel>

#include <frontsinglefilter.h>
#include <frontbanktransfer.h>

#include <definitionsignalblock.h>
#include <utilities.h>

#include <junctionbankdatabase.h>

namespace Ui {
class FrontSignalPanel;
}

class FrontSignalPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontSignalPanel(QWidget *parent = 0);
    ~FrontSignalPanel();

private:
    Ui::FrontSignalPanel *ui;
};

#endif // FRONTSIGNALPANEL_H

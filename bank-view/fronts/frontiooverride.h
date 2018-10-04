#ifndef FRONTIOOVERRIDE_H
#define FRONTIOOVERRIDE_H

#include <QWidget>
#include <QSqlTableModel>
#include <frontcommon.h>
#include <definitionauxiliarykeys.h>
//#include <modbuschannel.h>

//#include <frontsinglefilter.h>
//#include <junctionbankdatabase.h>

#include <utilities.h>

//#include <definitionsignalblock.h>
//#include <controllermanualmode.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontIoOverride;
}



class FrontIoOverride : public FrontCommon /*,
        public AbstractAuthReceiver*/
{
    Q_OBJECT

public:
    explicit FrontIoOverride(QWidget *parent = 0);
    ~FrontIoOverride();

    void Setup(QSqlTableModel* input,
               QSqlTableModel* output,
               QSqlTableModel* region,
               QSqlTableModel *header);

protected slots:
    void onViewSelectionChanged(QModelIndex index);
    void onTimerTimeout();
    void onOverrideClicked(QModelIndex index);
protected:
    Ui::FrontIoOverride *ui;
};

#endif // FRONTIOOVERRIDE_H

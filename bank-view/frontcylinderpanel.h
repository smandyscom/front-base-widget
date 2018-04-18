#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <QWidget>
#include <abstractsqltableadpater.h>
#include <definitionbasicblocks.h>
#include <controllermanualmode.h>

#include <frontbanktransfer.h>
#include <frontsinglefilter.h>

#include <junctionbankdatabase.h>

#include <definitioncylinderblock.h>
#include <definitionioattributes.h>

#include <QSqlRecord>

namespace Ui {
class FrontCylinderPanel;
}

using namespace CylinderBlock;
using namespace IoAttributes;

class FrontCylinderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(QWidget *parent = 0);
    ~FrontCylinderPanel();
protected slots:
    void onCylinderCommandClicked();
    void onViewSelectionChanged();
    //!
    //! \brief onTimerTimeout
    //!
    void onTimerTimeout();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontCylinderPanel *ui;

    QSqlRelationalTableModel* __cylinderTable;

    ControllerManualMode* __controller;

    QList<CylinderBlock::DataBaseHeaders> __inputFields;
    QList<CylinderBlock::DataBaseHeaders> __outputFields;

    QString generateFilterString(QString key,QList<QString> conditions);

    QTimer* __timer;

    MODBUS_WORD __currentViewIndex;

};

#endif // FRONTCYLINDERPANEL_H

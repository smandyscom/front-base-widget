#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(AbstractQVariantSqlTable *cylinderTable,
                                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontCylinderPanel),
    __cylinderTable(cylinderTable)
{
    ui->setupUi(this);

    connect(ui->pushButtonGoA,SIGNAL(clicked(bool)),this,SLOT(onClicked()));
}


FrontCylinderPanel::~FrontCylinderPanel()
{
    delete ui;
}

void FrontCylinderPanel::onClicked()
{
   AbstractDataBlock adb =  __cylinderTable->RowRecord(0).value<AbstractDataBlock>();
   __cylinderTable->RowRecord(0,QVariant::fromValue(adb));
}


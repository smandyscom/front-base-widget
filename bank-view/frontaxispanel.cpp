#include "frontaxispanel.h"
#include "ui_frontaxispanel.h"

FrontAxisPanel::FrontAxisPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontAxisPanel)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                          JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                          utilities::trimNamespace(QVariant::fromValue(AxisBlock::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(AxisBlock::NAME)),
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_AXIS,
                          ui->widgetTransfer);
    //!
    ui->tableViewAxis->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS));
    //!
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_AXIS),
                                                     ui->tableViewAxis);
}

FrontAxisPanel::~FrontAxisPanel()
{
    delete ui;
}

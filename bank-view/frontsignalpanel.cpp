#include "frontsignalpanel.h"
#include "ui_frontsignalpanel.h"

FrontSignalPanel::FrontSignalPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSignalPanel)
{
    ui->setupUi(this);
    //!
    FrontSingleFilter* __fsf = new FrontSingleFilter(ui->widgetFilter);
    __fsf->DataTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS));
    __fsf->PrimaryTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION));
    //!
    ui->tableViewSignal->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS));
    //!
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_SIGNALS),
                                                     ui->tableViewSignal);
}

FrontSignalPanel::~FrontSignalPanel()
{
    delete ui;
}

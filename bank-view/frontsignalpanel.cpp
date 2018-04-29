#include "frontsignalpanel.h"
#include "ui_frontsignalpanel.h"

FrontSignalPanel::FrontSignalPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSignalPanel)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS),
                          JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                          utilities::trimNamespace(QVariant::fromValue(SignalBlock::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(SignalBlock::NAME)),
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_SIGNAL,
                          ui->widgetTransfer);
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

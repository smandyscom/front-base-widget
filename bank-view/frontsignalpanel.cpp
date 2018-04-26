#include "frontsignalpanel.h"
#include "ui_frontsignalpanel.h"

FrontSignalPanel::FrontSignalPanel(QSqlRelationalTableModel *signalTable,
                                   QSqlRelationalTableModel *regionTable,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSignalPanel)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(signalTable,
                          regionTable,
                          utilities::trimNamespace(QVariant::fromValue(SignalBlock::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(SignalBlock::NAME)),
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_SIGNAL,
                          ui->widgetTransfer);
    //!
    ui->tableViewSignal->setModel(signalTable);
}

FrontSignalPanel::~FrontSignalPanel()
{
    delete ui;
}

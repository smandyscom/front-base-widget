#include "frontsafetypanel.h"
#include "ui_frontsafetypanel.h"
#include <QDebug>


FrontSafetyPanel::FrontSafetyPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSafetyPanel)
{
    ui->setupUi(this);
	
    //! Auth
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxInterlockBypass);
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxLightgateBypass);
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxAirChoke);
}

FrontSafetyPanel::~FrontSafetyPanel()
{
    delete ui;
}

void FrontSafetyPanel::Setup(QSqlTableModel* input,
	QSqlTableModel* output,
	QSqlTableModel* header)
{
	//!
	ui->tableViewMonitor->setModel(input);
	HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,
		ui->tableViewMonitor);
	//!
	ui->tableViewOverride->setModel(output);
	HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,
		ui->tableViewOverride);
	
}

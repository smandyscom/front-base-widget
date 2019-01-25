#include "frontsafetypanel.h"
#include "ui_frontsafetypanel.h"
#include <QDebug>


FrontSafetyPanel::FrontSafetyPanel(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontSafetyPanel)
{
    ui->setupUi(this);
	
	m_auth->addDisableWidget(AUTH::ROLE_OPERATOR, this);
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

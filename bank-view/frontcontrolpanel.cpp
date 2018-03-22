#include "frontcontrolpanel.h"
#include "ui_frontcontrolpanel.h"

frontControlPanel::frontControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frontControlPanel)
{
    ui->setupUi(this);
}

frontControlPanel::~frontControlPanel()
{
    delete ui;
}

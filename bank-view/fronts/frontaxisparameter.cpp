#include "frontaxisparameter.h"
#include "ui_frontaxisparameter.h"

FrontAxisParameter::FrontAxisParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontAxisParameter)
{
    ui->setupUi(this);
}

FrontAxisParameter::~FrontAxisParameter()
{
    delete ui;
}

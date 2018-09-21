#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

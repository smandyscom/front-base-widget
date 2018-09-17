#include "fronttwinfilter2.h"
#include "ui_fronttwinfilter2.h"

FrontTwinFilter2::FrontTwinFilter2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontTwinFilter2)
{
    ui->setupUi(this);
}

FrontTwinFilter2::~FrontTwinFilter2()
{
    delete ui;
}

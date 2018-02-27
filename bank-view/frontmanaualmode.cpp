#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"

FrontManaualMode::FrontManaualMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode)
{
    ui->setupUi(this);
}

FrontManaualMode::~FrontManaualMode()
{
    delete ui;
}

#include "frontbankmanager.h"
#include "ui_frontbankmanager.h"

FrontBankManager::FrontBankManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontBankManager)
{
    ui->setupUi(this);
}

FrontBankManager::~FrontBankManager()
{
    delete ui;
}

void FrontBankManager::onUpdatedFromPlc()
{

}

void FrontBankManager::onOperationPerformed()
{

}

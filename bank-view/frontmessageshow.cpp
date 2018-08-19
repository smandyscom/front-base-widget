#include "frontmessageshow.h"
#include "ui_frontmessageshow.h"


//!
//! \brief FrontMessageShow::FrontMessageShow
//! \param parent
//! Receive
FrontMessageShow::FrontMessageShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontMessageShow)
{
    ui->setupUi(this);
    __receiver = new MessageReceiver(this);
    connect(__receiver,SIGNAL(messageReceivedFormatted(QString)),this,SLOT(onMessageReceived(QString)));
}

FrontMessageShow::~FrontMessageShow()
{
    delete ui;
}

void FrontMessageShow::lineCheck()
{
    //ui->textBrowser->
}


void FrontMessageShow::onMessageReceived(QString arg)
{
    ui->textBrowser->append(arg);
}

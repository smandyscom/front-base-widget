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
    m_receiver = new MessageReceiver(this);
    connect(m_receiver,SIGNAL(messageReceivedFormatted(QString)),this,SLOT(onMessageReceived(QString)));

    ui->plainTextEdit->document()->setMaximumBlockCount(50); //limited buffer
}

FrontMessageShow::~FrontMessageShow()
{
    delete ui;
}

void FrontMessageShow::onMessageReceived(QString arg)
{
    ui->plainTextEdit->appendPlainText(arg);
}

#include "messagereceiver.h"
#include <messagehandler.h>

MessageReceiver::MessageReceiver(QObject *parent) : QObject(parent)
{
    MessageHandler::Instance()->registerReceiver(this);
}

MessageReceiver::~MessageReceiver()
{
    MessageHandler::Instance()->unregisterReceiver(this);
}


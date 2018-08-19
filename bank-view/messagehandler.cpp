#include "messagehandler.h"

MessageHandler::MessageHandler(QObject *parent) : QObject(parent)
{
    __default = qInstallMessageHandler(&messageHandler);
}

//!
//! \brief ControllerMessageHandler::messageHandler
//! Formatting message
//! Dispatch to all receiver
void MessageHandler::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &str)
{
    __default(type,context,str);

    QString __formatted = QString("%1[%2]%3")
            .arg(QDateTime::currentDateTime().toString(timeformat))
            .arg(QVariant::fromValue(Message::Type(type)).toString())
            .arg(str);

    //! Dispatch messages
    foreach (MessageReceiver* var, __list) {
        emit var->messageReceivedFormatted(__formatted);
    }
}

void MessageHandler::registerReceiver(MessageReceiver *arg)
{
    __list.append(arg);
}

QList<MessageReceiver*> MessageHandler::__list;
QString MessageHandler::timeformat = "yyyyMMddhhmmss";
MessageHandler* MessageHandler::__instance = nullptr;
QtMessageHandler MessageHandler::__default = nullptr;

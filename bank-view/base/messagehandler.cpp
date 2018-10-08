#include "messagehandler.h"

MessageHandler::MessageHandler(QObject *parent) : QObject(parent)
{
    m_default = qInstallMessageHandler(&messageHandler);
}

//!
//! \brief ControllerMessageHandler::messageHandler
//! Formatting message
//! Dispatch to all receiver
void MessageHandler::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &str)
{
#ifndef NDEBUG
	//IF define RELEASE , do not output to Debugger/Console
	m_default(type,context,str);
#endif // !NDEBUG

    QString formatted = QString("%1[%2]%3")
            .arg(QDateTime::currentDateTime().toString(timeformat))
            .arg(QVariant::fromValue(Message::Type(type)).toString())
            .arg(str);

    //! Dispatch messages
    foreach (MessageReceiver* var, m_list) {
        emit var->messageReceivedFormatted(formatted);
    }
}

void MessageHandler::registerReceiver(MessageReceiver *arg)
{
    m_list.append(arg);
//    connect(arg,SIGNAL(destroyed(QObject*)),[=](){
//        //remove from list
//        __list.removeOne(arg);
//    });
}
void MessageHandler::unregisterReceiver(MessageReceiver *arg)
{
    m_list.removeOne(arg);
}

QList<MessageReceiver*> MessageHandler::m_list;
QString MessageHandler::timeformat = "yyyyMMddhhmmss";
MessageHandler* MessageHandler::m_instance = nullptr;
QtMessageHandler MessageHandler::m_default = nullptr;

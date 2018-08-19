#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <messagereceiver.h>

namespace Message{
Q_NAMESPACE
enum Type
{
    Debug = QtDebugMsg,
    Warning = QtWarningMsg,
    System = QtSystemMsg,
    Critical = QtCriticalMsg,
    Fatal = QtFatalMsg,
    Info = QtInfoMsg,
};
Q_ENUM_NS(Type)
}

class MessageHandler : public QObject
{
    Q_OBJECT
public:

    static MessageHandler* Instance()
    {
        if(__instance==nullptr)
            __instance = new MessageHandler;
        return __instance;
    }


    static void registerReceiver(MessageReceiver* arg);
    static void unregisterReceiver(MessageReceiver* arg);
signals:

public slots:

protected :
    explicit MessageHandler(QObject *parent = nullptr);


    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &str);
    static QList<MessageReceiver*> __list;
    static QString timeformat;
    static MessageHandler* __instance;
    static QtMessageHandler __default;
};

#endif // MESSAGEHANDLER_H

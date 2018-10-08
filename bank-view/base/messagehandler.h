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
        if(m_instance==nullptr)
            m_instance = new MessageHandler;
        return m_instance;
    }


    static void registerReceiver(MessageReceiver* arg);
    static void unregisterReceiver(MessageReceiver* arg);
signals:

public slots:

protected :
    explicit MessageHandler(QObject *parent = nullptr);


    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &str);
    static QList<MessageReceiver*> m_list;
    static QString timeformat;
    static MessageHandler* m_instance;
    static QtMessageHandler m_default;
};

#endif // MESSAGEHANDLER_H

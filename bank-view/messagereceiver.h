#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>

class MessageReceiver : public QObject
{
    Q_OBJECT
public:
    explicit MessageReceiver(QObject *parent = nullptr);
    ~MessageReceiver();

signals:
    void messageReceivedFormatted(QString arg);
    void messageReceivedRaw(QtMsgType type, const QMessageLogContext &context, const QString &str);
public slots:
};

#endif // MESSAGERECEIVER_H

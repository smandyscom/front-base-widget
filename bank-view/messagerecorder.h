#ifndef MESSAGERECORDER_H
#define MESSAGERECORDER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDate>
#include <QtConcurrent>
#include <messagereceiver.h>
#include <messagehandler.h>

class MessageRecorder : public QObject
{
    Q_OBJECT
public:

    static MessageRecorder* Instance()
    {
        if(__instance ==nullptr)
            __instance = new MessageRecorder();

        return __instance;
    }

protected slots:
    void onMessageReceived(QString arg);
protected:
    explicit MessageRecorder(QObject *parent = nullptr);
    MessageReceiver* __receiver;

    void fileConcluded(QFile* file, QTextStream *stream);
    void fileOpenByDate();

    QString __logDirectory = "Log";

    QDate __currentDate;
    QTextStream* __stream;
    QFile* __currentFile;

    static MessageRecorder* __instance;
};

#endif // MESSAGERECORDER_H

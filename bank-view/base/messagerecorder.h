#ifndef MESSAGERECORDER_H
#define MESSAGERECORDER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDate>
#include <qtextstream.h>

#include <messagereceiver.h>
#include <messagehandler.h>

class MessageRecorder : public QObject
{
    Q_OBJECT
public:

    static MessageRecorder* Instance()
    {
        if(m_instance ==nullptr)
            m_instance = new MessageRecorder();

        return m_instance;
    }

protected slots:
    void onMessageReceived(QString arg);
protected:
    explicit MessageRecorder(QObject *parent = nullptr);
    MessageReceiver* m_receiver;

    void fileConcluded(QFile* file, QTextStream *stream);
    void fileOpenByDate();

    QString m_logDirectory;

    QDate m_currentDate;
    QTextStream* m_stream;
    QFile* m_currentFile;

    static MessageRecorder* m_instance;
};

#endif // MESSAGERECORDER_H

#include "messagerecorder.h"
#include <QDebug>
MessageRecorder::MessageRecorder(QObject *parent) : QObject(parent)
{
   //check if Log/ existed
   if(!QDir(__logDirectory).exists())
       QDir().mkdir(__logDirectory);

   //open log file
   fileOpenByDate();

   //link
   __receiver = new MessageReceiver(this);
   connect(__receiver,SIGNAL(messageReceivedFormatted(QString)),this,SLOT(onMessageReceived(QString)));

}

void MessageRecorder::onMessageReceived(QString arg)
{
    if(QDate::currentDate() != __currentDate)
    {
        //conclude file , and open a new one
        QtConcurrent::run(this,&fileConcluded,__currentFile,__stream);
        fileOpenByDate();   //replace
    }

    *__stream << QString("%1\n").arg(arg); //write a line
    __stream->flush();
    //maintain log files
}

void MessageRecorder::fileConcluded(QFile *file,QTextStream *stream)
{
    stream->flush();
    file->close();
    delete file;
    delete stream;
}

void MessageRecorder::fileOpenByDate()
{
    __currentFile = new QFile(QString("%1/%2.log")
                              .arg(__logDirectory)
                              .arg(QDate::currentDate().toString("yyyyMMdd")));
    if(!__currentFile->open(QFile::Append))
        qWarning() << __currentFile->fileName() << "not existed" ;
    __stream = new QTextStream(__currentFile);
    __currentDate = QDate::currentDate();
//    __stream.setDevice(__currentFile); //would call flush for old device
}

MessageRecorder* MessageRecorder::__instance = nullptr;

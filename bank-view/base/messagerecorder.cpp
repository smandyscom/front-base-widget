#include "messagerecorder.h"
#include <QDebug>
MessageRecorder::MessageRecorder(QObject *parent) : 
	QObject(parent),
	m_logDirectory("Log")
{
   //check if Log/ existed
   if(!QDir(m_logDirectory).exists())
       QDir().mkdir(m_logDirectory);

   //open log file
   fileOpenByDate();

   //link
   m_receiver = new MessageReceiver(this);
   connect(m_receiver,SIGNAL(messageReceivedFormatted(QString)),this,SLOT(onMessageReceived(QString)));

}

void MessageRecorder::onMessageReceived(QString arg)
{
    if(QDate::currentDate() != m_currentDate)
    {
        //conclude file , and open a new one
        //QtConcurrent::run(this,&fileConcluded,__currentFile,__stream);
        /*QtConcurrent::run([=]()
        {*/
            fileConcluded(m_currentFile,m_stream);
        //});
        fileOpenByDate();   //replace
    }

    *m_stream << QString("%1\n").arg(arg); //write a line
    m_stream->flush();
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
    m_currentFile = new QFile(QString("%1/%2.log")
                              .arg(m_logDirectory)
                              .arg(QDate::currentDate().toString("yyyyMMdd")));
    if(!m_currentFile->open(QFile::Append))
        qWarning() << m_currentFile->fileName() << "not existed" ;
    m_stream = new QTextStream(m_currentFile);
    m_currentDate = QDate::currentDate();
//    __stream.setDevice(__currentFile); //would call flush for old device
}

MessageRecorder* MessageRecorder::m_instance = nullptr;



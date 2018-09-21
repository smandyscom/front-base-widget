#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QStyle>

#include <QSqlDatabase>
#include <QSqlError>

class CommonHelper : public QObject
{
    Q_OBJECT
public:
    explicit CommonHelper(QObject *parent = nullptr);

    static void setStyle(const QString &style) {
            QFile qss(style);
            qDebug() << QDir::currentPath();
            qss.open(QFile::ReadOnly);
            qApp->setStyleSheet(qss.readAll());
            qss.close();
        }
    static void setStyle(const QString &style,QWidget* target) {
            QFile qss(style);
            qDebug() << QDir::currentPath();
            qDebug() << qss.fileName();
            qss.open(QFile::ReadOnly);
            target->setStyleSheet(qss.readAll());
            qDebug() << target->styleSheet();
            target->style()->unpolish(target);
            target->style()->polish(target);
            qss.close();
        }
    static void openSqlite3Database(const QString path)
    {
        qDebug() << QDir::currentPath();
        QSqlDatabase m_database = QSqlDatabase::addDatabase("QSQLITE");//setup driver
        m_database.setDatabaseName(path);
        if(!m_database.open())
            qDebug() << m_database.lastError().text();
    }

signals:

public slots:
};

#endif // COMMONHELPER_H

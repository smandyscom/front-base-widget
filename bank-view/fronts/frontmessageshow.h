#ifndef FRONTMESSAGESHOW_H
#define FRONTMESSAGESHOW_H

#include <QWidget>
#include <messagereceiver.h>
#include <messagehandler.h>
namespace Ui {
class FrontMessageShow;
}


//!
//! \brief The FrontMessageShow class
//! Tasks
//! Receive and show message
//! Once total line over some specification , discard ( retain constant memory usage
class FrontMessageShow : public QWidget
{
    Q_OBJECT

public:
    explicit FrontMessageShow(QWidget *parent = 0);
    ~FrontMessageShow();

protected slots:
    void onMessageReceived(QString arg);
private:
    Ui::FrontMessageShow *ui;
protected:
    MessageReceiver* m_receiver;
};

#endif // FRONTMESSAGESHOW_H

#ifndef FRONTSAFETYPANEL_H
#define FRONTSAFETYPANEL_H

#include <QWidget>
#include <QTimer>

#include <definitionioattributes.h>
#include <definitionauxiliarykeys.h>
#include <frontcommon.h>
#include <utilities.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontSafetyPanel;
}

class FrontSafetyPanel : public FrontCommon
//        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontSafetyPanel(QWidget *parent = 0);
    ~FrontSafetyPanel();

	void Setup(QSqlTableModel* input,
		QSqlTableModel* output,
		QSqlTableModel* header);
private:
    Ui::FrontSafetyPanel *ui;
};

#endif // FRONTSAFETYPANEL_H

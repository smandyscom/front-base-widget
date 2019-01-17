#ifndef FRONTMAINPANEL_H
#define FRONTMAINPANEL_H

#include <QWidget>
#include <QSqlTableModel>

#include <frontcommon.h>
//#include <junctionbankdatabase.h>

#include <definitionmaincontrolblock.h>
#include <definitionmanualblock.h>

using namespace HEADER_STRUCTURE;

namespace Ui {
class FrontMainPanel;
}

class FrontMainPanel :
        public FrontCommon
{
    Q_OBJECT

public:
    enum MainStates
    {
        STATE_AUTO,
        STATE_SEMI_AUTO,
        STATE_MANUAL,
    };
    Q_ENUM(MainStates)
    enum PresetProperty
    {
        RELATED_BIT,
        MAINSTATE,
		ERROR_STATE,
    };
    Q_ENUM(PresetProperty)
    Q_PROPERTY(MainStates MAINSTATE MEMBER m_state)

    explicit FrontMainPanel(QWidget *parent = 0);
    ~FrontMainPanel();

    void Setup(QSqlTableModel* axisTable,
               QSqlTableModel* cylinderTable,
               QSqlTableModel* unitTable,
               QSqlTableModel* axisErrorTable,
               QSqlTableModel* cylinderErrorTable,
               QSqlTableModel* unitErrorTable,
               QSqlTableModel* typeTable,
		QSqlTableModel* monitor,
		QSqlTableModel* monitorHeader);
protected slots:
    void onButtonClicked();
	void onDelayTimeout();
protected:


    Ui::FrontMainPanel *ui;

    MainStates mainState(bool isPause,bool isManual);
    MainStates m_state;
    //!
    //! \brief errorDescription
    //! \return
    //!
    QString errorDescription(int deviceCategrory,int deviceIndex,int errorCode) ;

    HEADER_STRUCTURE::Headers locale;
    QSqlTableModel* m_deviceTable;
    //!
    //! \brief __deviceMap
    //! Key , Device categrory
    QMap<MODBUS_S_WORD,QSqlTableModel*> m_errorDeviceMap;
    //!
    //! \brief __errorDescriptionMap
    //! Key 1: Device categrory
    //! Key 2: Error Code
    QMap<MODBUS_S_WORD,QSqlTableModel*> m_errorCodeMap;

    void dynamicPropertyChanged(int key,QVariant value) Q_DECL_OVERRIDE;

	QSqlTableModel* m_monitorTable;

	QTimer* m_delayTimer;
};

#endif // FRONTMAINPANEL_H

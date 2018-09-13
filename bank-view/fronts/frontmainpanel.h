#ifndef FRONTMAINPANEL_H
#define FRONTMAINPANEL_H

#include <QWidget>

#include <frontcommon.h>
#include <junctionbankdatabase.h>

#include <definitionmaincontrolblock.h>

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

    explicit FrontMainPanel(QWidget *parent = 0);
    ~FrontMainPanel();

protected:
    Ui::FrontMainPanel *ui;

    MainStates mainState(bool isPause,bool isManual);
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

    void dynamicPropertyChanged(int key) Q_DECL_OVERRIDE;

};

#endif // FRONTMAINPANEL_H

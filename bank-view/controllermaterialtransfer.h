#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>
#include <QStateMachine>

#include <QSqlRelationalTableModel>
#include <QFileInfo>
#include <QtConcurrent>
#include <QElapsedTimer>

#include <modbuschannel.h>
#include <definitionslotblock.h>
#include <abstractsqltableadpater.h>

//!
//! \brief The ControllerMaterialTransfer class
//! Proceding material/slot handshaking
class ControllerMaterialTransfer :
        public QStateMachine
{
    Q_OBJECT
public:
    enum Grade
    {
        OK=1,
        BYPASS=3,
        NG=0,
    };
    enum NameConstants
    {
        //! Data base name
        //!
        MAT_DATA_SLOT,
        MAT_HEADER_SLOT
    };
    enum SlotContext
    {
        //! PLC->DB
        //PLC_ENGAGED=0x4008000,
        ACT=0x4018000,
        IS_VALID=0x4028000,
        WORD_OUT=0x4008000,
        SYNC_ACTION=0x4008001,
        //! PLC<-DB
        DB_ENGAGED=0x4008008,
        WORD_IN=0x4008008,
        DONE=0x4018008,
        //! Mutual
        MATERIAL_ID=0x4008010, //move to first 8 words
        BLOCK_DATA=0x4008014,

        //!
        MATERIAL_OVERRIDE=0x4050003, //material override
    };
    enum SyncRequests
    {
        ACTION_UPDATE_HEADER = 0x01,
        ACTION_UPDATE_BLOCK = 0x02,
        ACTION_CREATE = 0x04,
        ACTION_QUERY = 0x08,
    };
    enum SlotState
    {
        WAIT_ACT_ON,
        WAIT_ACT_OFF,
        TRANS,
    };
    Q_ENUM(SlotContext)
    Q_ENUM(SyncRequests)
    Q_ENUM(SlotState)
    Q_ENUM(NameConstants)
    Q_ENUM(Grade)

    explicit ControllerMaterialTransfer(int slotOffset,
                                        int channelIndex,
                                        QObject *parent = nullptr);
    ~ControllerMaterialTransfer();

    void Role(SyncRequests role){ m_role = role;}

    //! Current material id this slot held
    int MaterialId() const
    {
        return m_materialId;
    }
    int Index() const
    {
        return __slotIndex;
    }
    bool IsValid() const
    {
        return __isValid;
    }
    bool ConnectionEngaged() const
    {
        return __connectionEngaged;
    }
//    SlotType Role() const
//    {
//        return __role;
//    }
    //!
    //! \brief MaterialOverride
    //! \param value
    //! Able to control material override via slot interface
    void MaterialOverride(bool value)
    {
        __channel->Access<bool>(toOffseteAddress(MATERIAL_OVERRIDE),value);
    }

    QSqlTableModel* Table() const
    {
        return __table;
    }

    static QSqlDatabase DataBase()
    {
        return __database;
    }


    void CounterClear()
    {
        m_totalCounter = 0;
        m_okCounter = 0;
        m_ngCounter = 0;
        emit dataUpdated();
    }
    int TotalCount() const {return m_totalCounter;}
    int OKCount() const {return m_okCounter;}
    int NGCount() const {return m_ngCounter;}
    qreal OKRate() const
    {
        if(m_totalCounter==0)
            return 0;
        return ((qreal)m_okCounter/(qreal)m_totalCounter) * 100;
    }
    qreal NGRate() const
    {
        if(m_totalCounter==0)
            return 0;
        return ((qreal)m_ngCounter/(qreal)m_totalCounter) * 100;
    }
    Grade CurrentGrade() const {return m_currentGrade;}
    void IndexGrades(int index1,int index2)
    {
        m_index_grade1 = index1;
        m_index_grade2 = index2;
    }

signals:
    void dataUpdated();
    void idUpdated();
public slots:
    void onAboutToLeave();
    void onInsert();
    void onQuery();
    void onUpdate();
protected slots:
    //!
    //! \brief onMonitorBlockReply
    //! \param event
    //! Looping
    void onReply();

protected:
    ModbusChannel* __channel;

    int __slotIndex;
    int __channelIndex;

    int m_materialId;
    int m_lastMaterialId;
    bool __isValid;
    //SlotType __role;

//    SyncRequests __request;

    QSqlTableModel* __table;

    ModbusDriverAddress toOffseteAddress(int base);

    AbstractSqlTableAdpater* __adpator;

    QMap<SlotState,QState*> __stateMap;
    SlotState __currentState;

    static QSqlDatabase __database;
    static QString __databaseName;

    bool __connectionEngaged;

    int __pollCyclic;

    AbstractDataBlock __adb;

    QElapsedTimer __procedureTimer;

    //! Counters
    int m_totalCounter;
    int m_okCounter;
    int m_ngCounter;
    Grade m_currentGrade;
    //! To indicate which value to count as OK/NG according to individual slot...bad smell
    int m_index_grade1;
    int m_index_grade2;

    SyncRequests m_role;
};


#endif // CONTROLLERMATERIALTRANSFER_H

#ifndef CONTROLLERMATERIALTRANSFER_H
#define CONTROLLERMATERIALTRANSFER_H

#include <QObject>
#include <QSqlTableModel>
#include <QFileInfo>
#include <QElapsedTimer>

#include <controllerbase.h>
#include <definitionslotblock.h>

#include <abstractsqltableadpater.h>

//!
//! \brief The ControllerMaterialTransfer class
//! Mission , Proceding material/slot handshaking , put into databse
//! And generate property to front
class ControllerMaterialTransfer :
        public ControllerBase
{
    Q_OBJECT
public:
	//! First field of MAT_DATA as grade
    enum Grade
    {
        OK=1,
        BYPASS=3,
        NG=0,
    };
    enum SyncRole
    {
        ROLE_UPDATE_HEADER = 0x01,
        ROLE_UPDATE_BLOCK = 0x02, //read from bus write-in db
        ROLE_CREATE = 0x04, //generate material id
        ROLE_QUERY = 0x08,  //read from DB write-in bus
    };
	enum Miscs
	{
		MAT_DATA_SLOT,
	};
    Q_ENUM(SyncRole)
    Q_ENUM(Grade)
	Q_ENUM(Miscs)

    explicit ControllerMaterialTransfer(quint8 clientId, quint16 baseOffset, int interval, QObject *parent);
    ~ControllerMaterialTransfer();

	//! Settle handling routine
	void Role(SyncRole role);
	//!Which table going to store
	void SlotIndex(int index);

    //! Current material id this slot held
   /* int MaterialId() const
    {
        return __materialId;
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
    }*/
//    SlotType Role() const
//    {
//        return __role;
//    }
    //!
    //! \brief MaterialOverride
    //! \param value
    //! Able to control material override via slot interface
    /*void MaterialOverride(bool value)
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
    }*/

	static void OpenDatabase();

signals:
	void actionRaised();
protected slots:
	virtual void onAcknowledged(InterfaceRequest ack);

	void onInsert();
    void onQuery();
    void onUpdate();
protected:
	int m_slotIndex;
    int m_materialId;
    bool m_isValid;

    //QSqlTableModel* m_table;

    AbstractSqlTableAdpater* m_adpator;

    static QSqlDatabase m_database;
    static QString m_databaseName;

    //! Counters
    /*int m_totalCounter;
    int m_okCounter;
    int m_ngCounter;
    Grade m_currentGrade;*/
    //! To indicate which value to count as OK/NG according to individual slot...bad smell
    /*int m_index_grade1;
    int m_index_grade2;*/
};


#endif // CONTROLLERMATERIALTRANSFER_H

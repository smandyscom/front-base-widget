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
    
    enum SyncRole
    {
		ROLE_NONE = 0,
        ROLE_UPDATE_HEADER = 0x01,
        ROLE_UPDATE_BLOCK = 0x02, //read from bus write-in db
        ROLE_CREATE = 0x04, //generate material id
        ROLE_QUERY = 0x08,  //read from DB write-in bus
    };
	enum Miscs
	{
		MAT_DATA_SLOT,
		//! Database header
		CLIENT_ID,
		BASE_OFFSET,
		INTERVAL,
		ROLE,
	};
    Q_ENUM(SyncRole)
	Q_ENUM(Miscs)

    explicit ControllerMaterialTransfer(quint8 clientId, quint16 baseOffset, int interval, QObject *parent);
    ~ControllerMaterialTransfer();

	//! Settle handling routine
	void Role(SyncRole role);
	//!Which table going to store
	void SlotIndex(int index);

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

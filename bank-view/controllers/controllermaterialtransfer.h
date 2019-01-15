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
    
    

    explicit ControllerMaterialTransfer(quint8 clientId, quint16 baseOffset, int interval, QObject *parent=nullptr);
    ~ControllerMaterialTransfer();

	void Setup(SlotDataBlock::SyncRole role, int index, AbstractSqlTableAdpater* adaptor);

	int Index() const;

signals:
	void actionRaised();
protected slots:
	virtual void onAcknowledged(InterfaceRequest ack);

	void onTableInsert();
    void onTableQuery();
    void onTableUpdate();
protected:
	int m_slotIndex;
    int m_materialId;
    bool m_isValid;
	bool m_lastActStatus;

    AbstractSqlTableAdpater* m_adpator;
	SlotDataBlock::SyncRole m_role;
};


#endif // CONTROLLERMATERIALTRANSFER_H

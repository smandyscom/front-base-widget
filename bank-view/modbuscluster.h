#ifndef MODBUSCLUSTER_H
#define MODBUSCLUSTER_H

#include <QObject>
#include <QModbusDataUnit>
#include <modbussegment.h>

//!
//! \brief The ModbusCluster class
//! define this atomic meomory segment (not dividable , read/write at once
class ModbusCluster : public QObject
{
    Q_OBJECT
public:
    enum Attribute : int
    {

        READ = 0x01, //
        WRITE = 0x02, //
    };
    Q_DECLARE_FLAGS(Attributes,Attribute)

    explicit ModbusCluster(quint16* innerBaseAddress,
                           size_t size,
                           size_t modBusStartAddress /*base offset in modbus holding register*/,
                           Attributes attribute,
                           QObject *parent = nullptr);

    //!1
    //! Interface for upper layer
    //!
    void commitCluster();
    //!
    //! \brief beginUpdateContent
    //! non-blocking , asynchrous
    void beginUpdateCluster(); //return a callback?
    int sizeOf() const { return size;}

    //! setIsPolling , determine whether this cluster is auto updating (polling

signals:
    //!
    //! \brief contentUpdated
    //! face to upper layer
    void clusterUpdated(); // emit when reply fetched, report cluster id?
    //!
    //! \brief sendRequest
    //! \param request
    //! face to lower layer
    void sendModbusRequest(const ModbusSegment* request); //emit to request handler
public slots:

protected slots:
    //!
    //! \brief dataUpdated
    //! \param reply
    //! used to receive updated content from lower layer
    void onModbusReplyReceived(QModbusDataUnit reply);

protected:

    //!
    //! \brief request
    //! The specific request belongs to this cluster
    ModbusSegment* request;
    Attributes attribute;

    quint16* innerBaseAddress;
    size_t size; // in quint16

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusCluster::Attributes)

#endif // CLUSTERHEADER_H

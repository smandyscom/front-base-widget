#ifndef MODBUSCLUSTER_H
#define MODBUSCLUSTER_H

#include <QObject>
#include <QModbusDataUnit>
#include <modbussegment.h>

//Cluster <-> object? define type as well?
//define this atomic meomory segment
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

    explicit ModbusCluster(quint16* baseAddress,
                           size_t size,
                           size_t registerAddress /*base offset in modbus holding register*/,
                           Attribute attribute,
                           QObject *parent = nullptr);

    //!1
    //! Interface to upper layer
    void commit();
    void beginUpdate(); //return a callback?
    const int sizeOf(){ return size;}

    //! setIsPolling , determine whether this cluster is auto updating (polling

signals:
    void updated(); // emit when reply fetched, report cluster id?
    void sendRequest(const ModbusSegment* request); //emit to request handler
public slots:

protected slots:
    void dataUpdated(QModbusDataUnit reply);

protected:

    ModbusSegment* request;
    Attributes attribute;

    quint16* baseAddress;
    size_t size; // in quint16

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ModbusCluster::Attributes)

#endif // CLUSTERHEADER_H

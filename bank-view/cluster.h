#ifndef CLUSTER_H
#define CLUSTER_H

#include <QObject>
#include <QModbusDataUnit>
#include <serializedmodbusclient.h>

//Cluster <-> object? define type as well?
//define this atomic meomory segment
class Cluster : public QObject
{
    Q_OBJECT
public:
    enum Attribute : int
    {
        READ = 0x01, //
        WRITE = 0x02, //
//        HEADER_OR_BLOCK=0x04, //0:header , 1:block , markup as dirty when reading at first place
    };
    Q_DECLARE_FLAGS(Attributes,Attribute)

    explicit Cluster(quint16* underlyingBaseAddress,
                           size_t size,
                           size_t offset /*base offset in modbus holding register*/,
                           Attribute attribute,
                           SerializedModbusClient* gateway,
                           QObject *parent = nullptr);

    //interface for upper layer

    void write(quint32 offset,const quint16* data,size_t sz);
    const quint16* read(quint32 offset);

signals:
    void updated(); // emit when reply fetched, report cluster id?
public slots:

protected slots:
    void dataUpdated(QModbusDataUnit reply);

protected:

    SegmentModbus* request;
    Attributes attribute;

    quint16* baseAddress;
    size_t size; // in quint16

    SerializedModbusClient* gateway;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ClusterHeader::Attributes)

#endif // CLUSTERHEADER_H

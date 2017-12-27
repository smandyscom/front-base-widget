#ifndef CLUSTERHEADER_H
#define CLUSTERHEADER_H

#include <QObject>
#include <QModbusDataUnit>
#include <serializedmodbusclient.h>

class ClusterHeader : public QObject
{
    Q_OBJECT
public:
    enum Attributes : int
    {
        READ_HEADER = 0x01,
        READ_BLOCK = 0x02,//markup as dirty when reading at first place
        WRITE_HEADER=0x04,
        WRITE_BLOCK=0x08
    };
    Q_DECLARE_FLAGS(Attributes);

    explicit ClusterHeader(QObject *parent = nullptr);

    //interface for upper layer

    void write(quint32 offset,const quint16* data,size_t sz);
    const quint16* read(quint32 offset);

signals:

public slots:

protected:

    SegmentModbus* request;

    bool isDirty;

    Attributes attribute;

    quint16* baseAddress;
    size_t size;

    SerializedModbusClient* gateway;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ClusterHeader::Attributes);

#endif // CLUSTERHEADER_H

#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QList>
#include <cluster.h>


//address : channel,type,holding register address(16bits)
//according register address to update cluster

//composite clusters
class channel : public QObject
{
    Q_OBJECT
public:
    explicit channel(QObject *parent = nullptr);

    void beginRead(int address); //raising asynchrous updating operation
    QVariant readData(int address);
    writeData(address,QVariant value);
signals:
    void clusterUpdated(int clusterId);
public slots:


protected:

    QList<Cluster*> clusterCollection; //index as cluster id
    quint16 memory[INT16_MAX];
};

#endif // CHANNEL_H

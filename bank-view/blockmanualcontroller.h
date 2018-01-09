#ifndef BLOCKMANUALCONTROLLER_H
#define BLOCKMANUALCONTROLLER_H

#include <QObject>
#include <modbuschannel.h>
#include <blockmanual.h>

//
class BlockManualController : public QObject
{
    Q_OBJECT
public:
    explicit BlockManualController(QObject *parent = nullptr);

signals:

public slots:

protected:
    void onStateEnter();
    void onStateExited();

    //targetless transition
    //no polling mechanism for state machine framework


    //post Event and guard
    //pay big attention on bit signal , raise event when bit value changes
    //beginRead() , read()->raise events for those changed bit signals


    //s1-----raisingBit[DONE]---->s2
    //!
    //! \brief updateProcessing
    //! slot for receiving channel's clusterUpdated signal
    //! then distribute detailed bit event to state machine
    //!
    void updateProcessing(int clusterId); //slot , would emit transition condiiton


    ModbusChannel* channel;
};

#endif // BLOCKMANUALCONTROLLER_H

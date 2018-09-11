#ifndef INTERFACECLIENT_H
#define INTERFACECLIENT_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include <utilities.h>
#include <interfacerequest.h>

//!
//! \brief The InterfaceClient class
//! Base class for all kind of clients (MODBUS/ADS)
class InterfaceClient : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceClient(QObject *parent = nullptr);

    void pushRequest(InterfaceRequest request);

    quint16* Cache(){return __cache;}

signals:
    void requestAcknowledged(InterfaceRequest acknowledge);
public slots:
    void onTimerStart() { __workingTimer->start();}
protected slots:
    virtual void onPopRequest() ;
protected:
    QTimer* __workingTimer; //driving consume
    QQueue<InterfaceRequest> __queue;
    //!
    //! \brief __cache
    //! data cache
    quint16* __cache;
    bool __isProcessing;
    void operationDone();
};

#endif // INTERFACECLIENT_H
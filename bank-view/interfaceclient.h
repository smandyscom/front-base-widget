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

    quint16* Cache(){return m_cache;}

signals:
    void requestAcknowledged(InterfaceRequest acknowledge);
public slots:
    void onTimerStart() { m_workingTimer->start();}
protected slots:
    virtual void onPopRequest() ;
protected:
    QTimer* m_workingTimer; //driving consume
    QQueue<InterfaceRequest> m_queue;
    //!
    //! \brief __cache
    //! data cache
    quint16* m_cache;
    bool m_isProcessing;
    void operationDone();
};

#endif // INTERFACECLIENT_H

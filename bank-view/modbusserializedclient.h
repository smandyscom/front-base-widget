#ifndef MODBUSSERIALIZEDCLIENT_H
#define MODBUSSERIALIZEDCLIENT_H

#include <QObject>
#include <QModbusClient>
#include <QQueue>

#include <QTimer>
#include <QPair>




//!
//! \brief The ModbusSerializedClient class
//! Make sure requests are processed in sequence
//! Face to certain server address
class ModbusSerializedClient : public QObject
{
    Q_OBJECT
public:
    enum AccessMethod
    {
        READ,
        WRITE,
        READ_WRITE,// reserved
    };
    Q_ENUM(AccessMethod)

    typedef QPair<QModbusDataUnit,AccessMethod> ModbusRequest;

    explicit ModbusSerializedClient(QModbusClient* driverReference = nullptr,
                                    int serverAddress=1,
                                    QObject *parent = nullptr);
    void pushRequest(const ModbusRequest* request);
signals:
    //!
    //! \brief readRequestDone
    //! emitted when READ request had processed
    void readRequestDone(const QModbusDataUnit);
protected slots:
    void onPopRequest();
protected:

    QQueue<const ModbusRequest*> requestQueue;

    int serverAddress;
    QModbusClient* driverReference; //should be initialed somewhere

    //ModbusSegment* request;
    //!
    //! \brief isProcessing
    //! whether is processing request
    bool isProcessing;

    QTimer* timer; //driving consumer
};




#endif // SEQUENTIALMODBUSCLIENT_H

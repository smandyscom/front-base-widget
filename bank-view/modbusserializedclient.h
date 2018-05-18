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

    explicit ModbusSerializedClient(int __serverAddress=1,
                                    QObject *parent = nullptr);
    void pushRequest(const ModbusRequest* request);

    bool IsProcessing() const {return __isProcessing;}
    int ProcessingCount() const {return requestQueue.count();}

    QModbusClient* Driver() const {return __driver;}
    void Driver(QModbusClient* value);


    ~ModbusSerializedClient();

signals:
    //!
    //! \brief readRequestDone
    //! emitted when request had processed
    void readRequestDone(const QModbusDataUnit);
protected slots:
    void onPopRequest();
    //!
    //! \brief onDriverStateChanged
    //! \param state
    //! Control whether start/stop handling timer
    void onDriverStateChanged(QModbusDevice::State state);
    void onDriverErrorOccured(QModbusDevice::Error error);
protected:

    QQueue<const ModbusRequest*> requestQueue;

    int __serverAddress;
    QModbusClient* __driver; //should be initialed somewhere
    //!
    //! \brief isProcessing
    //! whether is processing request
    bool __isProcessing;

    QTimer* __timer; //driving consumer
};

#endif // SEQUENTIALMODBUSCLIENT_H

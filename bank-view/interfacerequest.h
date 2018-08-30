#ifndef INTERFACEREQUEST_H
#define INTERFACEREQUEST_H

#include <QVariant>

#define ADDRESS_MODE uint
#define ADDRESS_CLIENT_ID(x) reinterpret_cast<quint8*>(&x)[3]
#define ADDRESS_BIT_ACCESSOR(x) 0x0001 << reinterpret_cast<quint8*>(&x)[2]
#define ADDRESS_REGISTER(x) reinterpret_cast<quint16*>(&x)[0]
//32bits[4 bytes] , client id[1 byte],bit accessor(16bits)[1 byte] , register address (16bits for each register)[2 byte]


class InterfaceRequest
{
public:
    enum InterfaceAccess : int
    {
        READ,
        WRITE,
    };

    //!
    //! \brief InterfaceRequest
    //! \param access
    //! \param address
    //! \param data
    //!
    InterfaceRequest(InterfaceAccess access,ADDRESS_MODE address,QVariant data) :
        __access(access),
        __address(address),
        __data(data)
    {}

    InterfaceAccess Access(){return __access;}
    QVariant& Data() { return __data;}
    //!
    //! \brief Address
    //! \return
    //!
    ADDRESS_MODE Address() const{ return __address;}

protected:
    InterfaceAccess __access;
    ADDRESS_MODE __address;
    QVariant __data;
};

#endif // INTERFACEREQUEST_H

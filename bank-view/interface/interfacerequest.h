#ifndef INTERFACEREQUEST_H
#define INTERFACEREQUEST_H

#include <QObject>
#include <QVariant>
#include <definitionsbaselayer.h>

using namespace BaseLayer;


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
    InterfaceRequest(){}

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
Q_DECLARE_METATYPE(InterfaceRequest)


#endif // INTERFACEREQUEST_H

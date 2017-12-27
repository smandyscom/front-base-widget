#include "clusterheader.h"

ClusterHeader::ClusterHeader(QObject *parent) : QObject(parent)
{

}


const quint16* ClusterHeader::read(quint32 offset)
{
    if((attribute | READ_BLOCK)>0){
        //mark as DIRTY
        //connect handler
    }
    else
    {

    }

}

void ClusterHeader::write(quint32 offset, const quint16 *data, size_t sz)
{
    if (!((attribute | WRITE_HEADER)>0 || (attribute | WRITE_BLOCK)>0))
            return ; // no write attribute

    memcpy(baseAddress,data,sizeof(quint16)*sz); //store into underlying memory
    gateway->pushRequest(request);
}

#ifndef _Connector_
#define _Connector_
#include "BusInterface.h"
#include "common.h"

class Connector
{
protected:
    BusInterface *bi;
public:
    Connector(BusInterface*, int);
    ~Connector();

    virtual int read(addr_r) = 0;
    virtual void write(int, addr_r) = 0;
};
#endif

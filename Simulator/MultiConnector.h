#ifndef _MultiConnector_
#define _MultiConnector_

#include "common.h"
#include "Connector.h"

class MultiConnector : public Connector
{
public:
    MultiConnector(BusInterface*, int);
    ~MultiConnector();

    virtual int read(addr_r);
    virtual void write(int, addr_r);
    virtual data_buffer multi_read(addr_r);
    virtual void multi_write(int, addr_r);
};
#endif

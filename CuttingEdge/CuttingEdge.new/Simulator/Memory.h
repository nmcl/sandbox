#ifndef _Memory_
#define _Memory_
#include "common.h"
#include "Connector.h"

#define MEMORY_SIZE 1000

class Memory : public Connector
{
    int contents[MEMORY_SIZE];
public:
    Memory(BusInterface*, int);
    ~Memory();
    virtual int read(addr_r);
    virtual void write(int, addr_r);
};
#endif

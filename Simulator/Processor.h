#include "common.h"
#include "Connector.h"

class Processor : public Connector
{
public:
    Processor(BusInterface*, int);
    ~Processor();
    int read(addr_r);
    void write(int, addr_r);
};

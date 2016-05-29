#include "Memory.h"

class Ram : public Memory
{
public:
    Ram(BusInterface*, int);
    ~Ram();
    virtual int read(addr_r);
    virtual void write(int, addr_r);
};


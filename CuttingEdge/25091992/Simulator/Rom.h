#include "Memory.h"

class Rom : public Memory
{
public:
    Rom(BusInterface*, int);
    ~Rom();
    virtual int read(addr_r);
    virtual void write(int, addr_r);
};

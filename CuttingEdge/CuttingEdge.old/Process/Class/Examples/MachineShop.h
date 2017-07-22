#ifndef MACHINESHOP_
#define MACHINESHOP_

#include <Process.h>

class MachineShop : public Process
{
public:
    MachineShop();
    ~MachineShop();

    void Body();
    void Await();
};

#endif

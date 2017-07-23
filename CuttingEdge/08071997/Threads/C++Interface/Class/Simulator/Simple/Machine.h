#ifndef MACHINE_
#define MACHINE_

#include <Random.h>
#include <Process.h>
#include "Object.h"
#include "Client.h"

#define MAX_MACHINES 10

class Machine : public Process
{
private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;

public:
    Machine(long, double, double);
    ~Machine();

    double MTTF, MTTR;
    double Availability;
    boolean Available;
    long MachineID, NumberOfObjects;
    ObjectData* ResidentObjects[MAX_OBJECTS];

    void Body();
};

#endif

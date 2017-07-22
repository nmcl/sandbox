#ifndef MACHINE_
#define MACHINE_

#include <Random.h>
#include <Process.h>
#include "Object.h"
#include "Client.h"

/* Instances of this class are the machines which comprise the simulated network. */

const int MAX_MACHINES = 10; // Maximum number of machines in the network.

class Machine : public Process
{
public:
    Machine (long, double, double);
    ~Machine ();

    double MTTF, MTTR;
    double Availability;
    boolean Available, ShouldReconfigure;
    long MachineID, NumberOfObjects;
    ObjectData* ResidentObjects[MAX_OBJECTS];

    void Merge ();
    void Body ();
    
private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;

    static NormalStream* ChangeBy;

    double OMTTF, OMTTR;
    Draw* AlterChars;
    double MarginOfError;

    void ChangeStatistics ();
};

#ifdef INIT_
#undef INIT_
    NormalStream* Machine::ChangeBy = 0;
#endif
#endif

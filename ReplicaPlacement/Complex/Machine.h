/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef OBJECT_H_
#  include "Object.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif


/* Instances of this class are the machines which comprise the simulated network. */

const int MAX_MACHINES = 10; // Maximum number of machines in the network.

class Machine : public Process
{
public:
    Machine (long, double, double);
    ~Machine ();

    void Merge ();
    void Body ();

    double MTTF, MTTR;
    double Availability;
    Boolean Available, ShouldReconfigure;
    long MachineID, NumberOfObjects;
    ObjectData* ResidentObjects[MAX_OBJECTS];
    
private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;

    static UniformStream* ChangeBy;

    double OMTTF, OMTTR;
    Draw* AlterChars;
    double MarginOfError;

    void ChangeStatistics ();
};

#endif

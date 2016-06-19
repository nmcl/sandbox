/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MACHINE_H_
#include "Machine.h"
#endif


Machine::Machine (long id, double failure, double repair)
{
    Available = true;
    MachineID = id;

    MTTF = failure;
    MTTR = repair;
    RepairTime = new UniformStream(0, MTTR);
    OperativeTime = new UniformStream(0, MTTF);
    NumberOfObjects = 0;
    // Availability = (1-(1/MTTF))*(1/MTTR); // Roughly !!
    Availability = MTTF/(MTTF + MTTR); // Limiting Availability (ref Trivedi)
    cerr << "Machine " << MachineID << " availability is " << Availability;
    cerr << " <" << MTTF << ", " << MTTR << ">\n";
}

Machine::~Machine () {}

void Machine::Body ()
{
    for(;;)
    {
	Hold((*OperativeTime)());
	Available = false;

#ifdef DEBUG
	cout << "Machine " << MachineID << " failing at time " << CurrentTime() << endl << flush;
#endif

	for (int i = 0; i < NumberOfObjects; i++) {
	    if (ResidentObjects[i]->User != 0) {
		ResidentObjects[i]->Lock = unlocked;
#ifdef DEBUG
		cout << "Machine " << MachineID << " activating user\n" << flush;
#endif
		ResidentObjects[i]->User->Activate();
	    }
	}

	Hold((*RepairTime)());

#ifdef DEBUG
	cout << "Machine " << MachineID << " repaired at time " << CurrentTime() << endl << flush;
#endif

	// Hold for max object recovery time
	Available = true;
    }
}


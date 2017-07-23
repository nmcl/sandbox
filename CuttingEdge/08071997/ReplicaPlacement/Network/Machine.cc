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

#ifndef CLIENT_H_
#include "Client.h"
#endif


extern Client* cl;

Machine::Machine (long id, double failure, double repair)
                                                        : Available(true),
							  MachineID(id),
							  MTTF(failure),
							  MTTR(repair),
							  NumberOfObjects(0)
{
    RepairTime = new UniformStream(0, MTTR);
    OperativeTime = new UniformStream(0, MTTF);

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

	for (int i = 0; i < NumberOfObjects; i++)
	{
	    if (ResidentObjects[i])
	    {
		ResidentObjects[i]->Lock = unlocked;
#ifdef DEBUG
		cout << "Machine " << MachineID << " activating user\n" << flush;
#endif
		for (int k = 0; k < MaxClients; k++)
		{
		    if (ResidentObjects[i]->User[k])
		    {
			ResidentObjects[i]->User[k]->Activate();
			ResidentObjects[i]->User[k] = 0;
		    }
		}
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

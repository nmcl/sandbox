#include "Machine.h"

Machine::Machine(long id, double failure, double repair)
{
    Available = true;
    MachineID = id;

    MTTF = failure;
    MTTR = repair;
    RepairTime = new UniformStream(0, MTTR);
    OperativeTime = new UniformStream(0, MTTF);
    NumberOfObjects = 0;
    Availability = (1-(1/MTTF))*(1/MTTR); // Roughly !!
    cerr << "Machine " << MachineID << " availability is " << Availability << "\n";
}

Machine::~Machine() {}

void Machine::Body()
{
    for(;;)
    {
	Hold((*OperativeTime)());
	Available = false;

	for (int i = 0; i < NumberOfObjects; i++)
	{
	    if (ResidentObjects[i]->User != 0)
	    {
		ResidentObjects[i]->Lock = unlocked;
		ResidentObjects[i]->User->Activate();
	    }
	}

	Hold((*RepairTime)());
	// Hold for max object recovery time
	Available = true;
    }
}


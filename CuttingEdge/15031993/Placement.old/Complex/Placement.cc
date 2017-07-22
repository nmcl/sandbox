/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PLACEMENT_H_
#include "Placement.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern Object* obj;
extern Machine* Sites[MAX_MACHINES];
extern int NumberAllowed;
extern double availability;

Placement::Placement () { HoldTime = new UniformStream(0, 400); }

Placement::~Placement () {}

/* Parameters: Machines, Objects to be placed, the maximum number of objects
 * in a replica group, and the required availability for the replica group.
 */

void Placement::RandomPlacementPolicy (Machine* Sites[], Object* obj, int NumberOfObjects, double Availability)
{
    RP = new RandomPolicy(Sites, obj, NumberOfObjects, Availability);
}

/* Parameters: Machines, Objects to be placed, require Availability for replica group,
 * and the expected read/write ratio for the replica group.
 */

void Placement::SimplePlacementPolicy (Machine* Sites[], Object* obj, double Availability,
				       double opratio, int NumberOfObjects)
{
    SP = new SimplePolicy(Sites, obj, Availability, opratio, NumberOfObjects);
}


/* This is the body of the Placement process. Unless dynamic reconfiguration
 * is turned on then this will do nothing.
 */

void Placement::Body ()
{
#ifdef DYNAMIC
    boolean ReconRequired;
#endif

    for (;;)
    {
	Hold((*HoldTime)());

#ifdef DYNAMIC
	ReconRequired = false;

	// first see if any reconfiguration is necessary
	for (int i = 0; i < MAX_MACHINES; i++)
	    if (Sites[i]->ShouldReconfigure)
		ReconRequired = true;

	if (ReconRequired)
	{
#ifdef DEBUG
	    cout << "Reconfiguration required\n" << flush;
#endif

	    // Can only reconfigure if objects are not in use.
	    // So try to lock objects exclusively.

	    if (obj->LockAll())
	    {
		obj->RemoveObjects();
		if (obj->UseRandom)
		    RandomPlacementPolicy(Sites, obj, NumberAllowed, availability);
		else
		    SimplePlacementPolicy(Sites, obj, availability, obj->GetReadWriteRatio(), NumberAllowed);

		// Once reconfiguration is done turn off 'flags' on each machine
		for (int j = 0; j < MAX_MACHINES; j++)
		    if (Sites[j]->ShouldReconfigure)
			Sites[j]->ShouldReconfigure = false;

		obj->UnlockAll();
	    }
#ifdef DEBUG
	    else
		cout << "Reconfiguration delayed as group in use\n" << flush;
#endif
	}
#endif
    }
}

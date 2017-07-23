/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef RANDOMPOLICY_H_
#include "RandomPolicy.h"
#endif


// Randomly place objects. Don't check availability requirement.
// Only ensure that one object per machine.

RandomPolicy::RandomPolicy (Machine* Sites[], Object* obj,
			    int NumberOfObjects, double Availability)
{
    Place = new UniformStream(0, MAX_MACHINES);
    long Assigned[MAX_MACHINES], x;
    boolean used, ok;
    int j, NumberToAssign = (int) (Availability * 10);

    if (NumberToAssign > NumberOfObjects)
	NumberToAssign = NumberOfObjects;
    else
	if (NumberToAssign < obj->GetMinimum())
	    NumberToAssign = (int) obj->GetMinimum();

    for (int i = 0; i < MAX_MACHINES; i++)
	Assigned[i] = -1;

    for (i = 0; i < NumberToAssign; i++)
    {
	ok = false;

	while (!ok)
	{
	    x = (long) (*Place)();
	    j = 0;
	    used = false;
	    while ((Assigned[j] != -1) && (!used))
	    {
		if (Assigned[j] == x)
		    used = true;
		else
		    j++;
	    }

	    if (!used)
	    {
		Assigned[j] = x;
		ok = true;
	    }
	}

	if (Sites[x]->NumberOfObjects < NumberToAssign)
	{
	    cerr << "Placing replica " << i << " on node " << x << "\n";
	    obj->Replicas[i].location = Sites[x];
	    Sites[x]->ResidentObjects[Sites[x]->NumberOfObjects] = &obj->Replicas[i];
	    Sites[x]->NumberOfObjects++;
	}
	else
	    i--;
    }

    obj->SetWriteQuorum(NumberToAssign);
}

RandomPolicy::~RandomPolicy () { delete Place; }

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


RandomPolicy::RandomPolicy (Machine* Sites[], Object* obj, int NumberOfObjects)
{
    Place = new UniformStream(0, MAX_MACHINES);
    long Assigned[MAX_MACHINES], x, toGet;
    boolean used, ok;
    int j;

    for (int i = 0; i < MAX_MACHINES; i++)
	Assigned[i] = -1;

    if (NumberOfObjects < obj->GetMinimum())
	toGet = obj->GetMinimum();
    else
	toGet = NumberOfObjects;

    for (i = 0; i < toGet; i++)
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

	if (Sites[x]->NumberOfObjects < NumberOfObjects)
	{
	    cerr << "Placing replica " << i << " on node " << x << "\n";
	    obj->Replicas[i].location = Sites[x];
	    Sites[x]->ResidentObjects[Sites[x]->NumberOfObjects] = &obj->Replicas[i];
	    Sites[x]->NumberOfObjects++;
	}
	else
	    i--;
    }

    obj->SetWriteQuorum(NumberOfObjects);
}

RandomPolicy::~RandomPolicy () { delete Place; }

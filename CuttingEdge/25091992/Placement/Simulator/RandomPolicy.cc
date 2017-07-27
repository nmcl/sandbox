#include <iostream.h>

#include "RandomPolicy.h"

// Randomly place objects. Don't check availability requirement.
// Only ensure that one object per machine.

RandomPolicy::RandomPolicy (Machine* Sites[], Object* obj)
{
    Place = new UniformStream(0, MAX_MACHINES);
    long Assigned[MAX_MACHINES];
    long x;
    boolean used, ok;
    int j;

    for (int i = 0; i < MAX_MACHINES; i++)
	Assigned[i] = -1;

    for (i = 0; i < MAX_OBJECTS; i++)
    {
	ok = false;

	while (!ok)
	{
	    x = (*Place)();
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

	if (Sites[x]->NumberOfObjects < MAX_OBJECTS)
	{
	    cerr << "Placing object " << i << " on node " << x << "\n";
	    obj->Replicas[i].location = Sites[x];
	    Sites[x]->ResidentObjects[Sites[x]->NumberOfObjects] = &obj->Replicas[i];
	    Sites[x]->NumberOfObjects++;
	}
	else
	    i--;
    }
}

RandomPolicy::~RandomPolicy () { delete Place; }

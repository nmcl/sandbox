#include <iostream.h>

#include "SimplePolicy.h"

// Assume write operations only for now. Policy should change for read or write.
SimplePolicy::SimplePolicy (Machine* Sites[], Object* obj, double Availability, double op_ratio)
{
    double possible = 0, toget;
    Machine* Sorted[MAX_MACHINES];
    Machine* temp;
    long NumberAssigned = 0;
    int i;

    for (i = 0; i < MAX_MACHINES; i++)
	Sorted[i] = Sites[i];

    for (i = 0; i < MAX_MACHINES; i++)
    {
	for (int j = MAX_MACHINES-1; j >= i; j--)
	{
	    if (Sorted[j]->Availability >= Sorted[i]->Availability)
	    {
		temp = Sorted[i];
		Sorted[i] = Sorted[j];
		Sorted[j] = temp;
	    }
	}
    }

    if (op_ratio < 0.5)
	toget = Availability;
    else
	toget = Availability*2;

    i = 0;
    while ((possible < toget) && (NumberAssigned < MAX_OBJECTS))
    {
	obj->Replicas[NumberAssigned].location = Sorted[i];
	Sorted[i]->ResidentObjects[Sorted[i]->NumberOfObjects] = &obj->Replicas[NumberAssigned];
	Sorted[i]->NumberOfObjects++;
	possible += Sorted[i]->Availability;
	cerr << "Placing object " << NumberAssigned << " on node " << Sorted[i]->MachineID << "\n";
	NumberAssigned++;
	i++;
    }

    cerr << "Write quorum is " << NumberAssigned << "\n";
    obj->SetWriteQuorum(NumberAssigned);
}
/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>
#include <fstream.h>

#ifndef SIMPLEPOLICY_H_
#  include "SimplePolicy.h"
#endif

#ifndef PLACEMENT_H_
#  include "Placement.h"
#endif


extern int NumberOfClients;
extern ofstream machineStatsFile;


SimplePolicy::SimplePolicy (Machine* Sites[], Object* obj,
			    double Availability, double op_ratio, int NumberOfObjects)
{
    double possible = 1.0, toget = 1.0 - Availability;
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
	    if (Sorted[j] && Sorted[j]->Availability >= Sorted[i]->Availability)
	    {
		temp = Sorted[i];
		Sorted[i] = Sorted[j];
		Sorted[j] = temp;
	    }
	}
    }

#ifdef DEBUG
    cout << "Reconfiguration view.\n" << endl;
    for (int j = 0; j < MAX_MACHINES; j++)
	if (Sorted[j])
	    cout << "Machine " << Sorted[j]->MachineID << " availability " << Sorted[j]->Availability << endl;
#else
#ifdef STATS
    double mean = 0.0;
    for (int j = 0; j < MAX_MACHINES; j++)
	if (Sorted[j])
	    mean += Sorted[j]->Availability;

    machineStatsFile << Process::CurrentTime() << " " << mean/MAX_MACHINES << endl;
#endif    
#endif

    if (op_ratio > criticalRatio)
	toget *= improveFactor;

#ifdef DEBUG
    cout << "\ntrying to get " << toget << endl;
#endif

    i = 0;
    do
    {
#ifdef DEBUG
	cout << "possible " << possible << endl;
	cout << "NumberAssigned " << NumberAssigned << endl;
	cout << "NumberOfObjects " << NumberOfObjects << endl;
	cout << "obj->GetMinimum() " << obj->GetMinimum() << endl;
#endif
	
	obj->Replicas[NumberAssigned].location = Sorted[i];
	Sorted[i]->ResidentObjects[Sorted[i]->NumberOfObjects] = &obj->Replicas[NumberAssigned];
	Sorted[i]->NumberOfObjects++;
	possible *= (1.0 - Sorted[i]->Availability);
	cerr << "Placing replica " << NumberAssigned << " on node " << Sorted[i]->MachineID << "\n";
	NumberAssigned++;
	i++;
    } while (((possible > toget) && (NumberAssigned < NumberOfObjects)) || (NumberAssigned < obj->GetMinimum()));

#ifdef DEBUG
    cout << "*finished placement*" << endl;
    cout << "possible " << possible << endl;
    cout << "NumberAssigned " << NumberAssigned << endl;
    cout << "NumberOfObjects " << NumberOfObjects << endl;
    cout << "obj->GetMinimum() " << obj->GetMinimum() << endl;
#endif

    obj->NumberOfObjects = NumberAssigned;
    obj->SetWriteQuorum(NumberAssigned);
}

/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef MACHINE_H_
#  include "Machine.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

#include <fstream.h>
#include <iostream.h>
#include <stdio.h>


extern Client* cl;

UniformStream* Machine::ChangeBy = 0;


Machine::Machine (long id, double failure, double repair)
                                                        : Available(TRUE),
							  MachineID(id),
							  ShouldReconfigure(FALSE),
							  MTTF(failure),
							  MTTR(repair),
							  OMTTF(failure),
							  OMTTR(repair),
							  NumberOfObjects(0)
{
    RepairTime = new UniformStream(0, MTTR);
    OperativeTime = new UniformStream(0, MTTF);
    AlterChars = new Draw(0.9); // probability that MTTF and MTTR will not change significantly
    MarginOfError = 0.1; // if MTTF or MTTR stray by more than this % then reconfigure object groups
    
    if (!ChangeBy)
	ChangeBy = new UniformStream(95, 105);
    
    // Availability = (1-(1/MTTF))*(1/MTTR); // Roughly !!

    Availability = MTTF/(MTTF + MTTR); // Limiting Availability (ref Trivedi)
    // Will change availability as it is biased towards recovery time.

    cerr << "Machine " << MachineID << " availability is " << Availability;
    cerr << " <" << MTTF << ", " << MTTR << ">\n";
}

Machine::~Machine () {}

void Machine::Merge ()
{
    for (int i = 0; i < NumberOfObjects; i++)
    {
	if ((ResidentObjects[i] == 0) && (i != MAX_OBJECTS))
	{
	    ResidentObjects[i] = ResidentObjects[i+1];
	    i--;
	}
    }
}

void Machine::Body ()
{
    double ttf = 0.0, ttr = 0.0;
#ifdef STATS
    ofstream of;
    char fileName[128];
    
    ::memset(fileName, '\0', 128);
    sprintf(fileName, "Machine_%d_out", MachineID);
    of.open(fileName, ios::out);
#endif
    
    for(;;)
    {
	Hold((*OperativeTime)());
	Available = FALSE;

	
#ifdef DEBUG
	cout << "Machine " << MachineID << " failing at time " << CurrentTime() << endl << flush;
#endif

#ifdef STATS
        of << Process::CurrentTime() << " " << Available << endl;
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
	Available = TRUE;

#ifdef STATS
        of << Process::CurrentTime() << " " << Available << endl;
#endif

#ifdef RANDSTATS
	ChangeStatistics();
#endif
    }
}

void Machine::ChangeStatistics ()
{
#ifdef RANDSTATS
    if ((*AlterChars)())
    {
#ifdef DEBUG
        cout << "\n**Machine reconfiguration in order**" << endl;
	cout << "Will change Machine " << MachineID << "'s MTTF and MTTR\n" << flush;
#endif

        MTTF = MTTF*((*ChangeBy)()/100);
	MTTR = MTTR*((*ChangeBy)()/100);

#ifdef DEBUG
	cout << "Original MTTR for Machine " << MachineID << " is " << OMTTR << "\n" << flush;
	cout << "Original MTTF for Machine " << MachineID << " is " << OMTTF << "\n" << flush;
	cout << "Current MTTR for Machine " << MachineID << " is " << MTTR << "\n" << flush;
	cout << "Current MTTF for Machine " << MachineID << " is " << MTTF << "\n" << flush;
#endif

	delete RepairTime;
	delete OperativeTime;
	RepairTime = new UniformStream(0, MTTR);
	OperativeTime = new UniformStream(0, MTTF);

	if (((MTTF < OMTTF - OMTTF*MarginOfError) || (MTTF > OMTTF + OMTTF*MarginOfError)) ||
	    ((MTTR < OMTTR - OMTTR*MarginOfError) || (MTTR > OMTTR + OMTTR*MarginOfError)))
	{
#ifdef DEBUG
	    cout << "Machine " << MachineID << " out of bounds\n" << flush;
#endif
	    OMTTF = MTTF;
	    OMTTR = MTTR;
	    Availability = MTTF/(MTTF + MTTR);
	    ShouldReconfigure = TRUE;
	}
	
#ifdef DEBUG
        cout << "**Configuration finished**\n" << endl;
#endif	
    }
#endif
}

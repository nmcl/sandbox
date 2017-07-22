#include "Machine.h"
#include "Client.h"

extern Client* cl;

Machine::Machine (long id, double failure, double repair)
{
    Available = true;
    MachineID = id;
    ShouldReconfigure = false;

    MTTF = failure;
    MTTR = repair;
    OMTTF = MTTF;
    OMTTR = MTTR;

    RepairTime = new UniformStream(0, MTTR);
    OperativeTime = new UniformStream(0, MTTF);
    AlterChars = new Draw(0.99); // probability that MTTF and MTTR will not change significantly
    NumberOfObjects = 0;
    MarginOfError = 0.4; // if MTTF or MTTR stray by more than this % then reconfigure object groups
    
    if (!ChangeBy)
	ChangeBy = new NormalStream(100, 10);
    
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
		if (ResidentObjects[i]->User != 0)
		{
		    ResidentObjects[i]->Lock = unlocked;
#ifdef DEBUG
		    cout << "Machine " << MachineID << " activating user\n" << flush;
#endif
		    ResidentObjects[i]->User->Activate();
		}
	    }
	}

	Hold((*RepairTime)());
	
#ifdef DEBUG
	cout << "Machine " << MachineID << " repaired at time " << CurrentTime() << endl << flush;
#endif


	// Hold for max object recovery time
	Available = true;

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
	    ShouldReconfigure = true;
	}
    }
#endif
}

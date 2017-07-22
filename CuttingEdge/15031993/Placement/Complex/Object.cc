#include <iostream.h>

#include "Object.h"
#include "Machine.h"
#include "Placement.h"

extern Placement* pl;
extern Machine* Sites[MAX_MACHINES];
extern double availability;
extern int NumberAllowed;

ObjectData::ObjectData ()
{
    Available = true;
    Lock = unlocked;
    location = 0;
}

ObjectData::~ObjectData () {}


// Actual Replicated Object class
Object::Object (Protocol* rp, long n_objects, double read_write, boolean prot)
{
    if (n_objects > MAX_OBJECTS)
    {
	cerr << "Warning::attempt to create " << n_objects << "\n";
	cerr << "Limit is " << MAX_OBJECTS << "\n";
	NumberOfObjects = -1;
	RepProt = 0;
    }
    else
    {
	NumberOfObjects = n_objects;
	RepProt = rp;
    }

    n_write = 0;
    n_read = 0;
    rw_ratio = 0.0;
    value = read_write;
    margin = 0.2;
    UseRandom = prot;
}

Object::~Object () {}

void Object::RemoveObjects ()
{
    for (int k = 0; k < NumberOfObjects; k++)
    {
	if (Replicas[k].location)
	{
	    for (int j = 0; j < Replicas[k].location->NumberOfObjects; j++)
	    {
		for (int i = 0; i < NumberOfObjects; i++)
		{
		    // If object assigned to machine then delete it before the group
		    // is reconfigured

		    if (Replicas[k].location->ResidentObjects[j] == &Replicas[i])
		    {
			Replicas[k].location->ResidentObjects[j] = 0;
			Replicas[k].location->NumberOfObjects--;
			Replicas[k].location->Merge();
		    }
		}
	    }

	    Replicas[k].location = 0;
	}
    }
}

void Object::Reconfigure (lock_code l)
{
    // Will only reconfigure group if it is unlocked (not in use) and rw_ratio
    // has changed by more then the upper or lower bounds.

//    if ((rw_ratio != 0) && (l == unlocked))
    if (l == unlocked)
    {
#ifdef DEBUG
	cout << "required r/w ratio is: " << value << "\n" << flush;
	cout << "actual r/w ratio is:   " << rw_ratio << "\n" << flush;
	cout << "upper ratio bound is:  " << (value - value*margin) << "\n" << flush;
	cout << "lower ratio bound is:  " << (value + value*margin) << "\n" << flush;
#endif

	if ((rw_ratio < (value - value*margin)) || (rw_ratio > (value + value*margin)))
	{
	    value = rw_ratio;

#ifdef DEBUG
	    cout << "R/W ratio out-of-bounds. Reconfiguring objects\n" << flush;
#endif
	    RemoveObjects();

	    if (UseRandom)
		pl->RandomPlacementPolicy(Sites, this, NumberAllowed, availability);
	    else
		pl->SimplePlacementPolicy(Sites, this, NumberAllowed, availability, value);
	}
    }
}

boolean Object::PerformOperation (lock_code l, Client* who)
{
    long quorum;
    long numberfree = 0;
    boolean result;

    if (l == read)
	quorum = RepProt->ReadQuorum;
    else
	quorum = RepProt->WriteQuorum;

    if (l == read) n_read++;
    else
	if (l == write) n_write++;

    if (n_write)
	rw_ratio = (double) n_read/n_write;

#ifdef DEBUG
    cout << "Operation requires " << quorum << " operational replica(s)\n" << flush;
#endif

    if (l != unlocked)
    {
	// Should check Client id and lock conflicts
	for (int i = 0; i < NumberOfObjects; i++)
	{
	    if ((Replicas[i].Lock == unlocked) && (Replicas[i].location != 0))
	    {
		if (Replicas[i].location->Available)
		    numberfree++;
	    }
	}

#ifdef DEBUG
	cout << "Number of objects free is " << numberfree << "\n" << flush;
#endif
    }
    else
    {
	numberfree = quorum; // Always possible to try to unlock group

#ifdef DEBUG
	cout << "Will attempt to unlock " << numberfree << " objects\n" << flush;
#endif
    }

    if (numberfree >= quorum)
    {
	for (int j = 0; j < NumberOfObjects; j++)
	{
	    if ((Replicas[j].Lock == unlocked) || (l == unlocked))
	    {
		if (Replicas[j].location != 0)
		{
		    if (Replicas[j].location->Available)
		    {
			if (l != unlocked)
			{
#ifdef DEBUG
			    cout << "Client " << who->ClientID << " using object on machine "
				<< Replicas[j].location->MachineID << "\n" << flush;
#endif
			    Replicas[j].User = who;
			    Replicas[j].Lock = l;
			}
			else
			{
			    // can only unlock those objects you locked!

			    if (Replicas[j].User) // is replica in use?
				if (Replicas[j].User->ClientID == who->ClientID)
				    Replicas[j].User = 0;
			    Replicas[j].Lock = l;
			}
		    }
		}
	    }
	}
	
	result = true;
    }
    else
	result = false;

#ifdef DYNAMIC
    if (result)
	Reconfigure(l);
#endif

    return result;
}

// Used by reconfiguration manager to gain exclusive access to objects
boolean Object::LockAll ()
{
    int i;

    for (i = 0; i < NumberOfObjects; i++)
	if (Replicas[i].Lock != unlocked)
	    return false;
   
    for (i = 0; i < NumberOfObjects; i++)
	Replicas[i].Lock = write;
}

void Object::UnlockAll ()
{
    for (int i = 0; i < NumberOfObjects; i++)
	Replicas[i].Lock = unlocked;
}

// This should be done on a per client basis too
boolean Object::CheckQuorum (lock_code l)
{
    long quorum;
    long operational = 0;

    if (l == read)
	quorum = RepProt->ReadQuorum;
    else
	quorum = RepProt->WriteQuorum;

    for (int i = 0; i < NumberOfObjects; i++)
    {
	if ((Replicas[i].Lock == l) && (Replicas[i].location != 0))
	{
	    if (Replicas[i].location->Available)
		operational++;
	    else
		Replicas[i].Lock = unlocked;
	}
    }

    if (operational >= quorum)
	return true;
    else
	return false;
}

void Object::SetReadQuorum (long RQ) { RepProt->SetReadQuorum(RQ); }
long Object::ReadQuorum (void) { return RepProt->ReadQuorum; }

void Object::SetWriteQuorum (long WQ) { RepProt->SetWriteQuorum(WQ); }
long Object::WriteQuorum (void) { return RepProt->WriteQuorum; }

long Object::GetMinimum () { return RepProt->GetMinimum(); }
double Object::GetReadWriteRatio () { return value; }

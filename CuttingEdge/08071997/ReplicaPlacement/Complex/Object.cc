/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef OBJECT_H_
#include "Object.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef PLACEMENT_H_
#include "Placement.h"
#endif


extern Placement* pl;
extern Machine* Sites[MAX_MACHINES];
extern double availability;
extern int NumberAllowed;

ObjectData::ObjectData ()
                        : Available(true),
			  Lock(unlocked),
			  location(0)
{
    for (int i = 0; i < MaxClients; i++)
	User[i] = 0;
}

ObjectData::~ObjectData () {}

boolean ObjectData::IsUsing (Client* who)
{
    boolean found = false;
    
    for (int i = 0; (i < MaxClients) && (!found); i++)
    {
	if (User[i])
	{
	    if (User[i]->ClientID == who->ClientID)
		found = true;
	}
    }
    
    return found;
}

long ObjectData::GetLoad ()
{
    long load = 0;
    
    for (int i = 0; i < MaxClients; i++)
    {
	if (User[i])
	    load++;
    }
    
    return load;
}


// Actual Replicated Object class

Object::Object (Protocol* rp, long n_objects, double read_write, boolean prot)
                                                                             : n_write(0.0),
									       n_read(0.0),
									       rw_ratio(0.0),
									       value(read_write),
									       margin(0.2),
									       UseRandom(prot),
									       NumberOfClients(0),
									       n_ops(Hysterisis),
									       averageTI(1.0),
									       firstAssignment(true)
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
	objectSpread = n_objects;
	NumberOfObjects = n_objects;
	RepProt = rp;
    }
}

Object::~Object () {}

double Object::GetAverageObjects ()
{
    double tempVal = (objectSpread/averageTI);
    objectSpread = NumberOfObjects;
    averageTI = 1.0;
    return tempVal;
}

void Object::SetAverageObjects (double n_val) { objectSpread = n_val; }
    
double Object::GetLoad (Client* who)
{
    double load = 0.0;
    
    for (int i = 0; i < NumberOfObjects; i++)
    {
	if (Replicas[i].IsUsing(who))
	    load += Replicas[i].GetLoad();
    }
    
    return load;
}

double Object::GetAverageLoad ()
{
    double load = 0.0;
    
    for (int i = 0; i < NumberOfObjects; i++)
	load += Replicas[i].GetLoad();
    
    return (load/NumberOfObjects);
}

long Object::GetReplicasInUse (Client* who)
{
    long numberUsers = 0;
    
    for (int j = 0; j < NumberOfObjects; j++)
    {
	if (Replicas[j].IsUsing(who))
	    numberUsers++;
    }
    
    return numberUsers;
}

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

    if (l == unlocked)
    {
#ifdef DEBUG
	cout << "required r/w ratio is: " << value << "\n" << flush;
	cout << "actual r/w ratio is:   " << rw_ratio << "\n" << flush;
	cout << "upper ratio bound is:  " << (value - value*margin) << "\n" << flush;
	cout << "lower ratio bound is:  " << (value + value*margin) << "\n" << flush;
#endif

	if (n_ops <= Hysterisis)
	{
#ifdef DEBUG
	    cout << "Will not reconfigure because of hysterisis" << endl;
#endif
	    return;
	}
	
	if ((rw_ratio < (value - value*margin)) || (rw_ratio > (value + value*margin)))
	{
	    value = rw_ratio;

#ifdef DEBUG
	    cout << "R/W ratio out-of-bounds. Reconfiguring objects\n" << flush;
#endif
	    RemoveObjects();
	    n_read = n_write = n_ops = 0;

	    if (UseRandom)
		pl->RandomPlacementPolicy(Sites, this, NumberAllowed, availability);
	    else
		pl->SimplePlacementPolicy(Sites, this, availability, value, NumberAllowed);

	    objectSpread += NumberOfObjects;
	    averageTI++;
	}
    }
}

Opcode Object::unlockGroup (Client* who)
{
    for (int k = 0; k < NumberOfObjects; k++)
    {
	int nUsers = 0; // determine number of users of replica

	for (int i = 0; i < MaxClients; i++)
	{
	    if (Replicas[k].User[i]) // is replica in use?
	    {
		nUsers++; // replica has a user
		
		if (Replicas[k].User[i]->ClientID == who->ClientID)
		{
#ifdef DEBUG
		    cout << "Unlocking replica " << k << " on behalf of client " << who->ClientID << endl;
#endif				    
		    Replicas[k].User[i] = 0;
		    nUsers--; // one of replicas users removed
		}
	    }
	}

	if (nUsers == 0) // anymore users?
	{
#ifdef DEBUG
	    cout << "Replica " << k << " is unlocked" << endl;
#endif		
	    Replicas[k].Lock = unlocked;
	}
    }

    return ok;
}
	 
void Object::checkGroup (long& numberfree, long& lockConflict, Client* who, lock_code l)
{
    for (int i = 0; i < NumberOfObjects; i++)
    {
	if ((Replicas[i].Lock == unlocked) && (Replicas[i].location != 0))
	{
	    if (Replicas[i].location->Available)
		numberfree++;
	}

	if ((Replicas[i].Lock != unlocked) && (Replicas[i].location != 0))
	{
	    boolean found = false;
	    
	    for (int j = 0; (j < MaxClients) && (!found); j++)
	    {
		if (Replicas[i].User[j])
		{
		    if ((Replicas[i].User[j]->ClientID == who->ClientID) ||
			((Replicas[i].Lock == read) && (l == read)))
		    {
#ifdef DEBUG
			cout << "Replica " << i << " can be locked" << endl;
#endif			
			found = true;
			numberfree++;
		    }
		    else
		    {
#ifdef DEBUG
			cout << "Lock conflict on replica " << i << endl;
#endif			
			found = true;
			lockConflict++;
		    }
		}
	    }
	}
    }
}

Opcode Object::lockGroup (Client* who, lock_code l, long quorum)
{
    int toLock = 0, indx = 0;

// Do this first for load balancing
    
    while ((toLock < quorum) && (indx < MAX_OBJECTS))
    {
	if ((Replicas[indx].Lock == unlocked) && (Replicas[indx].location != 0))
	{
	    if (Replicas[indx].location->Available)
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " using object on machine "
		     << Replicas[indx].location->MachineID << "\n" << flush;
#endif
		Replicas[indx].User[0] = who; // can use first slot as no users of replica
		Replicas[indx].Lock = l;
		toLock++;
	    }
	}

	indx++;
    }

    if (toLock < quorum)
    {
	indx = 0;
	
	while (toLock < quorum)
	{
	    if ((Replicas[indx].Lock != unlocked) && (Replicas[indx].location != 0))
	    {
		if (Replicas[indx].location->Available)
		{
		    boolean found = false, lockOK = (boolean) (Replicas[indx].Lock == l);
		    int freeSlot = 0;
		
		    for (int i = 0; (i < MaxClients) && (!found); i++)
		    {
			if (Replicas[indx].User[i])
			{
			    if (Replicas[indx].User[i]->ClientID == who->ClientID)
			    {
#ifdef DEBUG
				cout << "Client " << who->ClientID << " already using object on machine "
				     << Replicas[indx].location->MachineID << "\n" << flush;
#endif
				toLock++;
				found = true;
			    }
			}
			else
			    freeSlot = i;
		    }
		    
		    if ((!found) && (lockOK))
		    {
#ifdef DEBUG
			cout << "Client " << who->ClientID << " using object on machine "
			     << Replicas[indx].location->MachineID << " which is currently locked" << endl;
#endif		    
			Replicas[indx].User[freeSlot] = who;
			toLock++;
		    }
		}
	    }
		
	    indx++;
	}
    }

    return ok;
}

Opcode Object::PerformOperation (lock_code l, Client* who)
{
    long quorum;
    long numberfree = 0, lockConflict = 0;
    Opcode result;
    
    if (l == read)
	quorum = RepProt->ReadQuorum;
    else
	quorum = RepProt->WriteQuorum;

    if (who->ClientID > NumberOfClients)
	NumberOfClients = who->ClientID;
    
#ifdef DEBUG
    cout << "Operation requires " << quorum << " operational replica(s)\n" << flush;
#endif

    if (l != unlocked)
    {
	checkGroup(numberfree, lockConflict, who, l);
	
#ifdef DEBUG
	cout << "Number of objects free is " << numberfree << "\n" << flush;
	cout << "Number of lock conflicts is " << lockConflict << endl;
#endif
    }
    else
	result = unlockGroup(who);
    
    if ((numberfree >= quorum) && (l != unlocked))
    {
	if (l == read)
	{
	    n_read++;
	    n_ops++;
	}
	else
	{
	    n_write++;
	    n_ops++;
	}
	
	if (n_write)
	    rw_ratio = (double) n_read/n_write;
	else
	    rw_ratio = INFINITE; // distinguish no write operations performed

	result = lockGroup(who, l, quorum);
    }
    else
    {
	if (l != unlocked)
	{
	    if (lockConflict >= quorum)
		result = lockconflict;
	    else
		result = failed;
	}
    }

#ifdef DYNAMIC
    if (result == ok)
	Reconfigure(l);
#endif

    return result;
}


// Used by reconfiguration manager to gain exclusive access to objects

boolean Object::LockAll ()
{
    for (int i = 0; i < NumberOfObjects; i++)
	if (Replicas[i].Lock != unlocked)
	    return false;
   
    for (int j = 0; j < NumberOfObjects; j++)
	Replicas[j].Lock = write;

    return true;
}

void Object::UnlockAll ()
{
    for (int i = 0; i < NumberOfObjects; i++)
	Replicas[i].Lock = unlocked;
}

boolean Object::CheckQuorum (lock_code l, Client* who)
{
    long quorum, operational = 0;

    if (l == read)
	quorum = RepProt->ReadQuorum;
    else
	quorum = RepProt->WriteQuorum;

    for (int i = 0; i < NumberOfObjects; i++)
    {
	if ((Replicas[i].Lock == l) && (Replicas[i].location != 0))
	{
	    if (Replicas[i].location->Available)
	    {
		if (Replicas[i].IsUsing(who))
		    operational++;
	    }
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
long Object::ReadQuorum () { return RepProt->ReadQuorum; }

void Object::SetWriteQuorum (long WQ)
{
    if (firstAssignment)
    {
	firstAssignment = false;
	objectSpread = WQ;
    }
    
    RepProt->SetWriteQuorum(WQ);
}

long Object::WriteQuorum () { return RepProt->WriteQuorum; }

long Object::GetMinimum () { return RepProt->GetMinimum(); }
double Object::GetReadWriteRatio () { return value; }

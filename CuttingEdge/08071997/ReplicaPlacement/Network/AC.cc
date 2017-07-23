/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef AC_H_
#include "AC.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif

#ifndef CLIENT_H_
#include "Client.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern Object* obj;


AC::AC (long RQ, long WQ, double RT) : Protocol(RQ, WQ, RT)
{
    name = "AvailableCopies";
}

AC::~AC () {}

void AC::SetWriteQuorum (long  WQ)
{
    WriteQuorum = WQ;
    SetReadQuorum(1);

    cout << "Write Quorum is " << WriteQuorum << "\n" << flush;
    cout << "Read Quorum is " << ReadQuorum << "\n" << flush;
}

void AC::SetReadQuorum (long RQ) { ReadQuorum = RQ; }

long AC::GetMinimum () { return 2; }

Opcode AC::PerformOperation (lock_code l, Client* who)
{
    long quorum;
    long numberfree = 0, lockConflict = 0;
    Opcode result;
    
    if (l == read)
	quorum = ReadQuorum;
    else
	quorum = WriteQuorum;

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
	result = lockGroup(who, l, quorum);
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

    return result;
}

boolean AC::CheckQuorum (lock_code l, Client* who)
{
    long quorum, operational = 0;

    if (l == read)
	quorum = ReadQuorum;
    else
	quorum = WriteQuorum;

    for (int i = 0; i < obj->NumberOfObjects; i++)
    {
	if ((obj->Replicas[i].Lock == l) && (obj->Replicas[i].location != 0))
	{
	    if (obj->Replicas[i].location->Available)
	    {
		if (obj->Replicas[i].IsUsing(who))
		    operational++;
	    }
	    else
		obj->Replicas[i].Lock = unlocked;
	}
    }

    if (operational >= quorum)
	return true;
    else
	return false;
}

Opcode AC::lockGroup (Client* who, lock_code l, long quorum)
{
    int toLock = 0, indx = 0;

// Do this first for load balancing
    
    while ((toLock < quorum) && (indx < MAX_OBJECTS))
    {
	if ((obj->Replicas[indx].Lock == unlocked) && (obj->Replicas[indx].location != 0))
	{
	    if (obj->Replicas[indx].location->Available)
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " using object on machine "
		     << obj->Replicas[indx].location->MachineID << "\n" << flush;
#endif
		obj->Replicas[indx].User[0] = who; // can use first slot as no users of replica
		obj->Replicas[indx].Lock = l;
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
	    if ((obj->Replicas[indx].Lock != unlocked) && (obj->Replicas[indx].location != 0))
	    {
		if (obj->Replicas[indx].location->Available)
		{
		    boolean found = false, lockOK = (boolean) (obj->Replicas[indx].Lock == l);
		    int freeSlot = 0;
		
		    for (int i = 0; (i < MaxClients) && (!found); i++)
		    {
			if (obj->Replicas[indx].User[i])
			{
			    if (obj->Replicas[indx].User[i]->ClientID == who->ClientID)
			    {
#ifdef DEBUG
				cout << "Client " << who->ClientID << " already using object on machine "
				     << obj->Replicas[indx].location->MachineID << "\n" << flush;
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
			     << obj->Replicas[indx].location->MachineID << " which is currently locked" << endl;
#endif		    
			obj->Replicas[indx].User[freeSlot] = who;
			toLock++;
		    }
		}
	    }
		
	    indx++;
	}
    }

    return ok;
}

void AC::checkGroup (long& numberfree, long& lockConflict, Client* who, lock_code l)
{
    for (int i = 0; i < obj->NumberOfObjects; i++)
    {
	if ((obj->Replicas[i].Lock == unlocked) && (obj->Replicas[i].location != 0))
	{
	    if (obj->Replicas[i].location->Available)
		numberfree++;
	}

	if ((obj->Replicas[i].Lock != unlocked) && (obj->Replicas[i].location != 0))
	{
	    boolean found = false;
	    
	    for (int j = 0; (j < MaxClients) && (!found); j++)
	    {
		if (obj->Replicas[i].User[j])
		{
		    if ((obj->Replicas[i].User[j]->ClientID == who->ClientID) ||
			((obj->Replicas[i].Lock == read) && (l == read)))
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

Opcode AC::unlockGroup (Client* who)
{
    for (int k = 0; k < obj->NumberOfObjects; k++)
    {
	int nUsers = 0; // determine number of users of replica

	for (int i = 0; i < MaxClients; i++)
	{
	    if (obj->Replicas[k].User[i]) // is replica in use?
	    {
		nUsers++; // replica has a user
		
		if (obj->Replicas[k].User[i]->ClientID == who->ClientID)
		{
#ifdef DEBUG
		    cout << "Unlocking replica " << k << " on behalf of client " << who->ClientID << endl;
#endif				    
		    obj->Replicas[k].User[i] = 0;
		    nUsers--; // one of replicas users removed
		}
	    }
	}

	if (nUsers == 0) // anymore users?
	{
#ifdef DEBUG
	    cout << "Replica " << k << " is unlocked" << endl;
#endif		
	    obj->Replicas[k].Lock = unlocked;
	}
    }

    return ok;
}

/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef PASSIVE_H_
#include "Passive.h"
#endif

#ifndef CLIENT_H_
#include "Client.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern Object* obj;

Passive::Passive (long RQ, long WQ, double RT) : Protocol(RQ, WQ/2+1, RT)
{
    name = "Primary Copy";
    US = new UniformStream(0.0, WQ);
    primaryCopy = (long) (*US)();
    cout << "Initial primary is " << primaryCopy << endl;
}

Passive::~Passive ()
{
    if (US)
	delete US;
}

void Passive::SetWriteQuorum (long WQ)
{
    WriteQuorum = WQ;
    SetReadQuorum(1);
    
    cout << "Write Quorum is " << WriteQuorum << endl;
    cout << "Read Quorum is " << ReadQuorum << endl;
}

void Passive::SetReadQuorum (long RQ) { ReadQuorum = RQ; }

long Passive::GetMinimum () { return 2; }

boolean Passive::checkGroup (Client* who, long quorum, lock_code l)
{
    long toget = 0;

    if (l == write)
    {
	boolean Owner = false;

	if (obj->Replicas[primaryCopy].Lock != unlocked)
	{
	    for (int i = 0; (i < MaxClients) && (!Owner); i++)
	    {
		if (obj->Replicas[primaryCopy].User[i])
		{
		    if (obj->Replicas[primaryCopy].User[i]->ClientID == who->ClientID)
			Owner = true;
		}
	    }
	}
	else
	    Owner = true;

	if (Owner)
	{
	    for (int j = 0; j < obj->NumberOfObjects; j++)
	    {
		if ((obj->Replicas[j].location->Available) && (obj->Replicas[j].Lock == unlocked))
		    toget++;
	    }
	}
    }
    else
    {
	if (obj->Replicas[primaryCopy].Lock != write)
	{
	    for (int k = 0; k < obj->NumberOfObjects; k++)
	    {
		if ((obj->Replicas[k].location->Available) && (obj->Replicas[k].Lock != write))
		    toget++;
	    }
	}
    }

    return (boolean) (toget >= quorum);
}

Opcode Passive::PerformOperation (lock_code l, Client* who)
{
    Opcode result;
    long quorum;
    
    who->primary = primaryCopy;
    
    if (!obj->Replicas[primaryCopy].location->Available)
    {
#ifdef DEBUG
	cout << "Primary " << primaryCopy << " has failed" << endl;
#endif	
	result = failed;
    }
    else
    {
	if (l == read)
	    quorum = ReadQuorum;
	else
	    quorum = WriteQuorum;
	
#ifdef DEBUG
	cout << "Operation requires " << quorum << " operational replica(s)" << endl;
#endif

	if (l != unlocked)
	{
	    if (checkGroup(who, quorum, l))
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " can lock group" << endl;
#endif		
		result = lockGroup(who, l, quorum);
	    }
	    else
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " cannot lock group" << endl;
#endif
		result = failed;
	    }
	}
	else
	    result = unlockGroup(who);
    }
    
    return result;
}

boolean Passive::CheckQuorum (lock_code l, Client* who)
{
    long quorum, operational = 0;
    
    if (l == read)
	quorum = ReadQuorum;
    else
	quorum = WriteQuorum;

    if ((!obj->Replicas[primaryCopy].location->Available) || (primaryCopy != who->primary))
    {
#ifdef DEBUG
	cout << "Primary " << primaryCopy << " has failed - client " << who->ClientID << " cannot acquire quorum" << endl;
#endif	
	if (!obj->Replicas[primaryCopy].location->Available)
	    electPrimary();
	return false;
    }

    for (int i = 0; i < obj->NumberOfObjects; i++)
    {
	if (obj->Replicas[i].Lock == l)
	{
	    if ((obj->Replicas[i].location->Available) && (obj->Replicas[i].IsUsing(who)))
		operational++;
	}
    }

    return (boolean) (operational >= quorum);
}

Opcode Passive::lockGroup (Client* who, lock_code l, long quorum)
{
    int indx = 0, toLock = 0;
    
    if (l == write)
    {
#ifdef DEBUG
	cout << "Client " << who->ClientID << " write locking primary " << primaryCopy << endl;
#endif	
	obj->Replicas[primaryCopy].Lock = l;
	obj->Replicas[primaryCopy].User[0] = who;
	toLock++;
    }
    
    while ((toLock < quorum) && (indx < MAX_OBJECTS))
    {
	if ((obj->Replicas[indx].Lock == unlocked) && (obj->Replicas[indx].location != 0))
	{
	    if (obj->Replicas[indx].location->Available)
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " using object on machine "
		     << obj->Replicas[indx].location->MachineID << endl;
#endif
		obj->Replicas[indx].User[0] = who;
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
				     << obj->Replicas[indx].location->MachineID << endl;
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
			     << obj->Replicas[indx].location->MachineID << endl;
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

Opcode Passive::unlockGroup (Client* who)
{
    for (int k = 0; k < obj->NumberOfObjects; k++)
    {
	int nUsers = 0;
	
	for (int i = 0; i < MaxClients; i++)
	{
	    if (obj->Replicas[k].User[i])
	    {
		nUsers++;
		
		if (obj->Replicas[k].User[i]->ClientID == who->ClientID)
		{
#ifdef DEBUG
		    cout << "Unlocking replica " << k << " on behalf of client " << who->ClientID << endl;
#endif
		    obj->Replicas[k].User[i] = 0;
		    nUsers--;
		}
	    }
	}
	
	if (nUsers == 0)
	{
#ifdef DEBUG
	    cout << "Replica " << k << " is unlocked" << endl;
#endif		
	    obj->Replicas[k].Lock = unlocked;
	}	    
    }
    
    return ok;
}

void Passive::electPrimary ()
{
    boolean elected = false, found = false;

    for (int i = 0; (i < obj->NumberOfObjects) && (!found); i++)
    {
	if (obj->Replicas[i].location->Available)
	    found = true;
    }

    if (!found)
    {
#ifdef DEBUG
	cout << "No replicas available for new primary" << endl;
#endif	
	return;
    }

    if (US)
	delete US;
    
    US = new UniformStream(0.0, obj->NumberOfObjects);

    while (!elected)
    {
	primaryCopy = (long) (*US)();
	if (obj->Replicas[primaryCopy].location->Available)
	{
#ifdef DEBUG
	    cout << "Replica " << primaryCopy << " elected as new primary" << endl;
#endif	    
	    elected = true;
	}
    }
}

    

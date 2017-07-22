/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef SINGLECOPY_H_
#include "SingleCopy.h"
#endif

#ifndef CLIENT_H_
#include "Client.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


extern Object* obj;

SingleCopy::SingleCopy (long RQ, long WQ, double RT) : Protocol(RQ, WQ/2+1, RT)
{
    name = "Primary Copy";
    US = new UniformStream(0.0, WQ);
    primaryCopy = (long) (*US)();
    cout << "Initial primary is " << primaryCopy << endl;
}

SingleCopy::~SingleCopy ()
{
    if (US)
	delete US;
}

void SingleCopy::SetWriteQuorum (long WQ)
{
    // both must be primary copy only

    WriteQuorum = 1;
    SetReadQuorum(1);
    
    cout << "Write Quorum is " << WriteQuorum << endl;
    cout << "Read Quorum is " << ReadQuorum << endl;
}

void SingleCopy::SetReadQuorum (long RQ) { ReadQuorum = RQ; }

long SingleCopy::GetMinimum () { return 2; }

boolean SingleCopy::checkGroup (Client* who, lock_code l)
{
    if (!obj->Replicas[primaryCopy].location->Available)
	return false;

    if (obj->Replicas[primaryCopy].Lock != unlocked)
    {
	if ((obj->Replicas[primaryCopy].Lock == read) && (l == read))
	{
#ifdef DEBUG
	    cout << "Primary is read locked so operation can be performed" << endl;
#endif	    
	    return true;
	}
	
	for (int i = 0; i < MaxClients; i++)
	{
	    if (obj->Replicas[primaryCopy].User[i])
	    {
		if (obj->Replicas[primaryCopy].User[i]->ClientID == who->ClientID)
		{
#ifdef DEBUG
		    cout << "Client " << who->ClientID << " is using this group already" << endl;
#endif		    
		    return true;
		}
	    }
	}

#ifdef DEBUG
	cout << "Primary is locked in conflicting manner" << endl;
#endif	
	return false;
    }
    else
    {
#ifdef DEBUG
	cout << "Primary is unlocked" << endl;
#endif	
	return true;
    }
}

Opcode SingleCopy::PerformOperation (lock_code l, Client* who)
{
    Opcode result;

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
	if (l != unlocked)
	{
	    if (checkGroup(who, l))
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " can lock group" << endl;
#endif		
		result = lockGroup(who, l);
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

boolean SingleCopy::CheckQuorum (lock_code l, Client* who)
{
    if ((!obj->Replicas[primaryCopy].location->Available) || (primaryCopy != who->primary))
    {
#ifdef DEBUG
	cout << "Primary " << primaryCopy << " has failed - client " << who->ClientID << " cannot acquire quorum" << endl;
#endif	
	if (!obj->Replicas[primaryCopy].location->Available)
	    electPrimary();
	return false;
    }

    if (obj->Replicas[primaryCopy].location->Available)
	return true;
    else
	return false;
}

Opcode SingleCopy::lockGroup (Client* who, lock_code l)
{
    int freeSlot;

    if (l == write)
    {
#ifdef DEBUG
	cout << "Client " << who->ClientID << " write locking primary " << primaryCopy << endl;
#endif	
	obj->Replicas[primaryCopy].Lock = l;
	obj->Replicas[primaryCopy].User[0] = who;
	return ok;
    }

    for (int i = 0; i < MaxClients; i++)
    {
	if (obj->Replicas[primaryCopy].User[i])
	{	
	    if (obj->Replicas[primaryCopy].IsUsing(who))
	    {
#ifdef DEBUG
		cout << "Client " << who->ClientID << " already using object on machine "
		     << obj->Replicas[primaryCopy].location->MachineID << endl;
#endif
		return ok;
	    }
	}
	else
	    freeSlot = i;
    }

#ifdef DEBUG
    cout << "Client " << who->ClientID << " using object on machine "
	 << obj->Replicas[primaryCopy].location->MachineID << endl;
#endif
    obj->Replicas[primaryCopy].User[freeSlot] = who;
    obj->Replicas[primaryCopy].Lock = l;
    return ok;
}

Opcode SingleCopy::unlockGroup (Client* who)
{
    int nUsers = 0;
	
    for (int i = 0; i < MaxClients; i++)
    {
	if (obj->Replicas[primaryCopy].User[i])
	{
	    nUsers++;
		
	    if (obj->Replicas[primaryCopy].User[i]->ClientID == who->ClientID)
	    {
#ifdef DEBUG
		cout << "Unlocking replica " << primaryCopy << " on behalf of client " << who->ClientID << endl;
#endif
		obj->Replicas[primaryCopy].User[i] = 0;
		nUsers--;
	    }
	}
    }
	
    if (nUsers == 0)
    {
#ifdef DEBUG
	cout << "Replica " << primaryCopy << " is unlocked" << endl;
#endif		
	obj->Replicas[primaryCopy].Lock = unlocked;
    }	    

    return ok;
}

void SingleCopy::electPrimary ()
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

    

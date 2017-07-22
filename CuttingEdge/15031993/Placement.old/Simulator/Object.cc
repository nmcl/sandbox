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


ObjectData::ObjectData ()
{
    Available = true;
    Lock = unlocked;
    location = 0;
}

ObjectData::~ObjectData () {}


// Actual Replicated Object class
Object::Object (Protocol* rp, long n_objects)
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
}

Object::~Object () {}

boolean Object::Valid (long i)
{
    if ((i < NumberOfObjects) && (i >= 0))
	return true;
    else
	return false;
}

boolean Object::PerformOperation (lock_code l, Client* who)
{
    long quorum;
    long numberfree = 0;

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
    }
    else
	numberfree = quorum; // Always possible to unlock group

#ifdef DEBUG
    cout << "Number of objects free is " << numberfree << "\n" << flush;
#endif

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
			}
			else
			    Replicas[j].User = 0;

			Replicas[j].Lock = l;
		    }
		}
	    }
	}

	return true;
    }
    else
	return false;
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

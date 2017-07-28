#include <iostream.h>

#include "Object.h"
#include "Machine.h"

ObjectData::ObjectData()
{
    Available = true;
    Lock = unlocked;
    location = 0;
}

ObjectData::~ObjectData() {}


// Actual Replicated Object class
Object::Object(Protocol* rp, long n_objects)
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
}

Object::~Object() {}

boolean Object::Valid(long i)
{
    if ((i < NumberOfObjects) && (i >= 0))
	return true;
    else
	return false;
}

boolean Object::PerformOperation(lock_code l, Client* who)
{
    long quorum;
    long numberfree = 0;

    if (l == read)
	quorum = RepProt->ReadQuorum;
    else
	quorum = RepProt->WriteQuorum;

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
			    Replicas[j].User = who;
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
boolean Object::CheckQuorum(lock_code l)
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

void Object::SetReadQuorum(long RQ) { RepProt->ReadQuorum = RQ; }

void Object::SetWriteQuorum(long WQ) { RepProt->WriteQuorum = WQ; }

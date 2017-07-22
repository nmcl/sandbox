#include <iostream.h>

#include "ReplicatedObject.h"
#include "List.h"
#include "Random.h"
#include "Machine.h"

extern Machine *Nodes[TotalMachines];
extern boolean Debug;
extern UniformStream *Rand1, *Rand2;

ObjectInfo::ObjectInfo()
{
    for (int i = 0; i < TotalMachines; i++, Map[i] = false);
}

ObjectInfo::~ObjectInfo() {}



// AvailabilityRequirement is the percentage of time that the object must be available
ReplicatedObject::ReplicatedObject(int x, double AR)
{
    int i = 0, attempt;

    Data.ID = x;
    Data.Mode = unlocked;

    AvailabilityRequirement = AR;
    for (int j = 0; j < TotalMachines; j++)
	Data.Map[j] = false;

    // Compute desired replica count and placement
    ReplicaCount = (int) (*Rand1)();

    // Place replicas (randomly) on separate machines
    while (i < ReplicaCount)
    {
	attempt = (int) (*Rand2)();
	if (!Data.Map[attempt])
	{
	    Data.Map[attempt] = true;
	    i++;
	}
    }

    // Compute read and write quorums based on count and protocol
    ReadQuorum = (ReplicaCount/2) +1;
    WriteQuorum = ReadQuorum;
}

ReplicatedObject::~ReplicatedObject() {}

// Check that the acquired replicas are still available
boolean ReplicatedObject::Check(ObjectInfo& data, lockcode Mode)
{
    int acquired = 0, required;

    for (int i = 0; i < TotalMachines; i++)
    {
	if (data.Map[i] && Nodes[i]->Available)
	    acquired++;

	if (data.Map[i] && !Nodes[i]->Available)
	    data.Map[i] = false;
    }

    if (Mode == write)
	required = WriteQuorum;
    else
	required = ReadQuorum;

    if (acquired >= required)
	return true;
    else
    {
	data.grant = false;
	return false;
    }
}

// This function locks the replicated object. Returns a map of the acquired replicas.

ObjectInfo ReplicatedObject::Lock(Transaction *Behalf, lockcode Mode)
{
    int acquired = 0;
    ObjectInfo data;
    boolean lockconflict = true;

    // Check for lock conflict
    if ((Data.Mode <= read) && (Mode != write) || ((Mode == write) && (Data.Mode == unlocked)))
	lockconflict = false;

    if (!lockconflict)
    {
	for (int i = 0; i < TotalMachines; i++)
	{
	    if (Data.Map[i] && Nodes[i]->Available)
	    {
		TList::Add(Nodes[i]->ActiveTransactions, Behalf);
		data.Map[i] = true;
		acquired++;
	    }
	}

	if (Mode == write)
	    needed = WriteQuorum;
	else
	    needed = ReadQuorum;

	// Set object lock mode
	Data.Mode = Mode;

	if (acquired >= needed)
	{
	    data.quorum = acquired;
	    grant = true;
	}
	else
	    grant = false;
    }
    else
	grant = false;

    data.grant = grant;

    if (Debug)
    {
	if (Mode == write)
	    cout << "Write ";
	else
	    cout << "Read ";
	cout << "Lock request on object " << Data.ID;
	cout << " for transaction " << Behalf->TransactionID();
	if (grant)
	    cout << " granted";
	else
	    cout << " refused";
	cout << "\n";
    }

    return data;
}

// Unlock the object
void ReplicatedObject::Unlock(Transaction *Behalf)
{
    for (int i = 0; i < TotalMachines; i++)
    {
	if (Data.Map[i] && Nodes[i]->Available)
	{
	    if (!TList::Empty(Nodes[i]->ActiveTransactions))
	    {
		for (TList* p = Nodes[i]->ActiveTransactions; p != 0; p = p->Next())
		{
		    if (p->Xaction->TransactionID() == Behalf->TransactionID())
			TList::Delete(Nodes[i]->ActiveTransactions, p);
		}
	    }
	}
    }
    
    Data.Mode = unlocked;
}

int ReplicatedObject::ObjectID() { return Data.ID; }

boolean ReplicatedObject::IsMapped(int i) { return Data.Map[i]; }

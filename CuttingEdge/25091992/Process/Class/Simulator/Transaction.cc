#include <iostream.h>

#include "Common.h"
#include "Transaction.h"
#include "List.h"
#include "Random.h"
#include "OA.h"

extern ReplicatedObject *Objects[TotalObjects];
extern int StartedTransactions, CompletedTransactions;
extern boolean Debug;
extern UniformStream *Rand3, *Rand4, *Rand5;
Draw *dw = new Draw((double) 1/ReadWriteRatio);

// Null body as all work is done by Body
Transaction::Transaction() {}

Transaction::~Transaction() {}

int Transaction::TransactionID() { return ID; }

void Transaction::Body()
{
    int i = 0;

    // Update the global counter and use it for a unique id
    StartedTransactions++;
    ID = StartedTransactions;
    p = 0;

    // Generate a compute time for this transaction
    ComputeTime = (int) (*Rand5)();

    //  Select a set of objects and access modes
    ObjectCount = (int) (*Rand3)();
    while (i < ObjectCount)
    {
	int attempt = (int) (*Rand4)();

	if (!OA::FindObj(Objects[attempt], p))
	{
	    lockcode lockmode;

	    if ((*dw)())
		lockmode = read;
	    else
		lockmode = write;

	    OA *oa_ptr = new OA(Objects[attempt], lockmode);
	    OList::Add(p, oa_ptr);
	    i++;
	}
    }

    if (Debug)
    {
	cout << "Created Transaction " << ID;
	cout << " which accesses " << ObjectCount;
	cout << " objects for " << ComputeTime << "\n";
    }

    // Launch another
    if (StartedTransactions < TotalTransactions)
    {
	Transaction* nt;
	nt = new Transaction();
	nt->Resume();
    }

    // Calculate completion time
    CompletionTime = Time() + ComputeTime;

    // Loop 'computing' until done
    if (Time() >= CompletionTime)
	Completed = true;
    else
	Completed = false;

    while (!Completed)
    {
	// If possible, acquire quorum of locks on objects
	OList *mk;
	for (mk = p; mk != 0; mk = mk->Next())
	    mk->ORef->data = mk->ORef->ObjRef->Lock(this, mk->ORef->Mode);

	// See if a quorum was achieved for every object
	LostQuorum = false;
	for (mk = p; mk != 0; mk = mk->Next())
	    if (!mk->ORef->data.grant)
		LostQuorum = true;

	while ((!Completed) && (!LostQuorum))
	{
	    // Compute until done or interrupted
	    Hold(CompletionTime - Time());

	    // Decide if we're done or just interrupted
	    if (Time() >= CompletionTime)
		Completed = true;
	    else
		Completed = false;

	    if (!Completed)
	    {
		// must have been interrupted - check objects
		for (mk = p; mk != 0; mk = mk->Next())
		    LostQuorum = mk->ORef->ObjRef->Check(mk->ORef->data, mk->ORef->Mode);
	    }
	}

	// Release all locks;
	for (mk = p; mk != 0; mk = mk->Next())
//	    if (mk->ORef->data.grant)
	    mk->ORef->ObjRef->Unlock(this);

	// If we need to go round again, wait a bit before attempting to re-acquire locks
	if (LostQuorum)
	{
	    CompletionTime = CompletionTime + Delta;
	    Hold(Delta);
	}
    }

    CompletedTransactions++;

    if (Debug)
	cout << "Completed Transaction " << ID << "\n";
}


#include <iostream.h>

#include "Common.h"
#include "List.h"
#include "Display.h"
#include "Machine.h"
#include "ReplicatedObject.h"

extern Machine *Nodes[TotalMachines];
extern ReplicatedObject *Objects[TotalObjects];

void Display::DumpMachines()
{
    TList *p;

    cout << " Machine      MTTF             MTTR         Speed    Active\n";
    cout << "          Mean    Std Dev  Mean    Std Dev           Transactions\n";

    for (int i = 0; i < TotalMachines; i++)
    {
	cout << " " << i;
	cout << "    " << Nodes[i]->machine_info->MTTFMean;
	cout << "    " << Nodes[i]->machine_info->MTTFStdDev;
	cout << "    " << Nodes[i]->machine_info->MTTRMean;
	cout << "    " << Nodes[i]->machine_info->MTTRStdDev;
	cout << "  " << Nodes[i]->machine_info->Speed;

	if (!TList::Empty(Nodes[i]->ActiveTransactions))
	{
	    for (p = Nodes[i]->ActiveTransactions; p != 0; p = p->Next())
		cout << "  " << p->Xaction->TransactionID();
	}

	cout << "\n";
    }
}

void Display::DumpObjects()
{
    cout << " Object  Replica  Read    Write    Machines\n";
    cout << "         Count    Quorum  Quorum\n";

    for (int i = 0; i < TotalObjects; i++)
    {
	cout << " " << Objects[i]->ObjectID();
	cout << "    " << Objects[i]->ReplicaCount;
	cout << "    " << Objects[i]->ReadQuorum;
	cout << "    " << Objects[i]->WriteQuorum;
	cout << "    ";

	for (int j = 0; j < TotalMachines; j++)
	    if (Objects[i]->IsMapped(j) == true)
		cout << " " << j;

	cout << "\n";
    }
}

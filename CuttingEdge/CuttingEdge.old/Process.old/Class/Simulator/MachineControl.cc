#include <Process.h>
#include "Random.h"
#include "Common.h"
#include "MachineControl.h"
#include "Transaction.h"

MachineControl::MachineControl(Machine* body) { TheMachine = body; }

MachineControl::~MachineControl() {}

void MachineControl::Body()
{
    TList *p;

    NormalStream* nsf = new NormalStream(TheMachine->machine_info->MTTFMean,
					 TheMachine->machine_info->MTTFStdDev);
    NormalStream* nsr = new NormalStream(TheMachine->machine_info->MTTRMean,
					 TheMachine->machine_info->MTTRStdDev);

    // Endlessly alternate between up and down
    for (;;)
    {
	// Process transactions until failure
	cerr << "MachineControl::Holding for failure\n";
	Hold((int) (*nsf)());

	// Crash
	TheMachine->Available = false;

	// Notify transactions that a machine has gone away
	for (p = TheMachine->ActiveTransactions; p != 0; p = p->Next())
	{
	    if (p->Xaction != 0)
	    {
		cerr << "MachineControl::reactivating transaction\n";
		p->Xaction->ReActivate();
	    }
	}

	if (TheMachine->ActiveTransactions != 0)
	    TList::Clear(TheMachine->ActiveTransactions);

	// Repair
	cerr << "MachineControl::Holding for repair\n";
	Hold((int) (*nsr)());

	// Recover
	TheMachine->Available = true;
    }
}

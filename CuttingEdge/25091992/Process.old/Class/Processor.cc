#include "Processor.h"

Processor::Processor(int value) { state = value; }

Processor::~Processor() {}

void Processor::Body()
{
    if (state == 1)
    {
	double i = 10.0;

	for (int j = 0; j < 10; j++)
	{
	    cout << "Processor::Suspended\n" << flush;
	    Hold(i);
	    cout << "Processor::Reactivated\n" << flush;

	    i += 10.0;
	}
    }
    else
    {
	ReActivateAt(55.0);
	Suspend();
	cerr << "Woken up\n";

	ReActivateAt(155.0);
	Suspend();
	cerr << "Woken up again\n";
    }
}


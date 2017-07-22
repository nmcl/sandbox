#include <iostream.h>

#include "Arrivals.h"
#include "Job.h"

Arrivals::Arrivals (double mean) { InterArrivalTime = new ExponentialStream(mean); }

Arrivals::~Arrivals () { delete InterArrivalTime; }

void Arrivals::Body ()
{
    for(;;)
    {
	Hold((*InterArrivalTime)());
	Job* work = new Job();
    }
}

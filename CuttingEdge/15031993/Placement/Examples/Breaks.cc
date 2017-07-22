#include "Breaks.h"
#include "Machine.h"
#include "Queue.h"

extern Machine* M;
extern Queue JobQ;

Breaks::Breaks ()
{
    RepairTime = new UniformStream(1, 10);
    OperativeTime = new UniformStream(20, 1000);
    interrupted_service = false;
}

Breaks::~Breaks ()
{
    delete RepairTime;
    delete OperativeTime;
}

void Breaks::Body ()
{
    for(;;)
    {
	Hold((*OperativeTime)());
	M->Broken();
	M->Cancel();

	if(!JobQ.IsEmpty())
	    interrupted_service = true;

	Hold((*RepairTime)());
	M->Fixed();
	if (interrupted_service)
	    M->ActivateAt(M->ServiceTime() + CurrentTime());
	else
	    M->ActivateAt();

	interrupted_service = false;
    }
}

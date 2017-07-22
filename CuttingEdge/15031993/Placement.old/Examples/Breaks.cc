/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BREAKS_H_
#include "Breaks.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef QUEUE_H_
#include "Queue.h"
#endif

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

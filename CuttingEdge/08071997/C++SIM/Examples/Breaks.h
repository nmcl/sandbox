/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BREAKS_H_
#define BREAKS_H_

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef PROCESS_H_
#include <Process.h>
#endif


/* Waits for a time interval drawn from the OperativeTime. Then kills of the
 * Machine and waits for a time interval drawn from the RepairTime before
 * restarting the Machine.
 */

class Breaks : public Process
{
public:
    Breaks ();
    ~Breaks ();

    void Body ();

private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;
    boolean interrupted_service;
};

#endif

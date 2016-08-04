/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BREAKS_H_
#define BREAKS_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif


/* Waits for a time interval drawn from the OperativeTime. Then kills of the
 * Process and waits for a time interval drawn from the RepairTime before
 * restarting the Process.
 */

class Breaks : public Process
{
public:
    Breaks (int id, RandomStream* ot, RandomStream* rt);
    ~Breaks ();

    void Body ();
private:
    RandomStream* repairTime;
    RandomStream* operativeTime;
    int breakID;
};

#endif

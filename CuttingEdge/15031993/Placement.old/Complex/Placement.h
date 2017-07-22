/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PLACEMENT_H_
#define PLACEMENT_H_

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef RANDOMPOLICY_H_
#include "RandomPolicy.h"
#endif

#ifndef SIMPLEPOLICY_H_
#include "SimplePolicy.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


const double criticalRatio = 5.0;
const double improveFactor = 0.0001;


/* This class calls the relevant placement policy. */

class Placement : public Process
{
public:
    Placement ();
    ~Placement ();

    void RandomPlacementPolicy (Machine*[], Object*, int, double);
    void SimplePlacementPolicy (Machine*[], Object*, double, double, int);

    void Body ();

protected:
    RandomPolicy* RP;
    SimplePolicy* SP;
    UniformStream* HoldTime;
};

#endif

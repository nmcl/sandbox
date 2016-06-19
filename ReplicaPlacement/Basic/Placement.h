/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PLACEMENT_
#define PLACEMENT_

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


/* This class calls the relevant placement placement policy. */

class Placement
{
public:
    Placement ();
    ~Placement ();

    void RandomPlacementPolicy (Machine*[], Object*, int, double);
    void SimplePlacementPolicy (Machine*[], Object*, int, double, double);

protected:
    RandomPolicy* RP;
    SimplePolicy* SP;
};

#endif

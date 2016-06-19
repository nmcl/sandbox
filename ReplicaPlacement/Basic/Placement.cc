/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PLACEMENT_H_
#include "Placement.h"
#endif


Placement::Placement () {}

Placement::~Placement () {}

/* Parameters: Machines, Objects to be placed, the maximum number of objects
 * in a replica group, and the required availability for the replica group.
 */

void Placement::RandomPlacementPolicy (Machine* Sites[], Object* obj, int NumberOfObjects, double Availability)
{
    RP = new RandomPolicy(Sites, obj, NumberOfObjects, Availability);
}

/* Parameters: Machines, Objects to be placed, require Availability for replica group,
 * and the expected read/write ratio for the replica group.
 */

void Placement::SimplePlacementPolicy (Machine* Sites[], Object* obj, int NumberOfObjects,
				       double Availability, double opratio = 0.9)
{
    SP = new SimplePolicy(Sites, obj, Availability, opratio, NumberOfObjects);
}

#ifndef PLACEMENT_
#define PLACEMENT_

#include "RandomPolicy.h"
#include "SimplePolicy.h"
#include "Machine.h"
#include "Object.h"

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

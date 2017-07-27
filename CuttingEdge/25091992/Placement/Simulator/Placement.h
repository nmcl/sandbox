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

    void RandomPlacementPolicy (Machine*[], Object*);
    void SimplePlacementPolicy (Machine*[], Object*, double, double);

protected:
    RandomPolicy* RP;
    SimplePolicy* SP;
};

#endif

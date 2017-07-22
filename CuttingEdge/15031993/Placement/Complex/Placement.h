#ifndef PLACEMENT_
#define PLACEMENT_

#include <Process.h>
#include <Random.h>

#include "RandomPolicy.h"
#include "SimplePolicy.h"
#include "Machine.h"
#include "Object.h"

/* This class calls the relevant placement placement policy. */

class Placement : public Process
{
public:
    Placement ();
    ~Placement ();

    void RandomPlacementPolicy (Machine*[], Object*, int, double);
    void SimplePlacementPolicy (Machine*[], Object*, int, double, double);

    void Body ();

protected:
    RandomPolicy* RP;
    SimplePolicy* SP;
    UniformStream* HoldTime;
};

#endif

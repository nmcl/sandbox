#ifndef PLACEMENT_
#define PLACEMENT_

#include "RandomPolicy.h"
#include "SimplePolicy.h"
#include "Machine.h"
#include "Object.h"

class Placement
{
protected:
    RandomPolicy* RP;
    SimplePolicy* SP;

public:
    Placement();
    ~Placement();

    void RandomPlacementPolicy(Machine*[], Object*);
    void SimplePlacementPolicy(Machine*[], Object*, double, double);
};

#endif

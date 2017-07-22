#ifndef RANDOMPOLICY_
#define RANDOMPOLICY_

#include <Random.h>
#include "Machine.h"
#include "Object.h"

// The Random Placement Policy.

class RandomPolicy
{
public:
    RandomPolicy (Machine*[], Object*, int, double);
    ~RandomPolicy ();

private:
    UniformStream* Place;
};

#endif

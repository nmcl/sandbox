#ifndef RANDOMPOLICY_
#define RANDOMPOLICY_

#include <Random.h>
#include "Machine.h"
#include "Object.h"

class RandomPolicy
{
private:
    UniformStream* Place;

public:
    RandomPolicy(Machine*[], Object*);
    ~RandomPolicy();
};

#endif

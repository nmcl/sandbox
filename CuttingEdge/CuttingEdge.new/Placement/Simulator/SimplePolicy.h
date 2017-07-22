#ifndef SIMPLEPOLICY_
#define SIMPLEPOLICY_

#include "Machine.h"
#include "Object.h"

// The Simple Placement Policy.

class SimplePolicy
{
public:
    SimplePolicy (Machine*[], Object*, double, double, int);
    ~SimplePolicy ();
};

#endif

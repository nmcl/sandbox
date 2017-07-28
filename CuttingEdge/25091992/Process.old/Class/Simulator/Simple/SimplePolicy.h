#ifndef SIMPLEPOLICY_
#define SIMPLEPOLICY_

#include "Machine.h"
#include "Object.h"

class SimplePolicy
{
public:
    SimplePolicy(Machine*[], Object*, double, double);
    ~SimplePolicy();
};

#endif

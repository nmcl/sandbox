/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef SIMPLEPOLICY_H_
#define SIMPLEPOLICY_H_

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


// The Simple Placement Policy.

class SimplePolicy
{
  public:
    SimplePolicy (Machine*[], Object*, double, double, int);
    ~SimplePolicy ();
};

#endif

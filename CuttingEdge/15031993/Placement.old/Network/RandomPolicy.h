/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef RANDOMPOLICY_H_
#define RANDOMPOLICY_H_

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


class RandomPolicy
{
public:
    RandomPolicy (Machine*[], Object*, int);
    ~RandomPolicy ();

private:
    UniformStream* Place;
};

#endif

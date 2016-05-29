/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef FILTER_H_
#define FILTER_H_

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif


class Filter : public Process
{
public:
    Filter (RandomStream* fd);
    ~Filter ();

    void Body ();
private:
    RandomStream* filterDist;
};

#endif

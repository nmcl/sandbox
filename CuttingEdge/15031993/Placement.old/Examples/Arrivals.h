/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef ARRIVALS_H_
#define ARRIVALS_H_

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef RANDOM_H_
#include <Random.h>
#endif


/* Controls the rate at which Jobs arrive at the Machine */

class Arrivals : public Process
{
public:
    Arrivals (double);
    ~Arrivals ();

    void Body ();

private:
    ExponentialStream* InterArrivalTime;
};

#endif

/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef RESULTS_H_
#define RESULTS_H_

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif

#ifndef QUNATILE_H_
#include <Stat/Quantile.h>
#endif

// Results gathers statistics about jobs


class Results : public Process
{
public:
    Results (double q);
    ~Results ();

    void Body ();
    Quantile getStats() const;
private:
    Quantile stats;
};

#endif

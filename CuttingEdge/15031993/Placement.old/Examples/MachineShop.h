/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MACHINESHOP_H_
#define MACHINESHOP_H_

#ifndef PROCESS_H_
#include <Process.h>
#endif


/* This class starts up all of the main processes involved in the simulation.
 * If the -DBREAKS option is added to the CFLAGS for the Makefile then the
 * Breaks process will be created to deactivate of the Machine process at
 * suitable intervals. Otherwise it will not be created and the Machine will
 * operate uninterrupted.
 */

class MachineShop : public Process
{
public:
    MachineShop ();
    ~MachineShop ();

    void Body ();
    void Await ();
};

#endif

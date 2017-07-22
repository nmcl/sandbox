/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef COMMON_H_
#include <common.h>
#endif

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef MACHINESHOP_H_
#include "MachineShop.h"
#endif


int main()
{
    Thread_Type::Initialize(); // Initialize the threads package.

    MachineShop m; // Start up the real main body of the simulation.
    m.Await(); // Suspend main's thread (NOTE: this MUST be done by all applications.)

    return 0;
}

/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef MAINBODY_H_
#include "MainBody.h"
#endif


int main(int argc, char* argv[])
{
    Thread::Initialize(); // Initialize threads package.

    MainBody m(argc, argv);
    m.Await(); // Suspend main thread. This MUST be done by all applications.
    m.Exit();

    return 0;
}


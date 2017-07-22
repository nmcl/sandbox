#include <iostream.h>
#include <Process.h>

#include "MainBody.h"

main(int argc, char* argv[])
{
    LWP_Thread::Initialize(); // Initialize threads package.

    if (argc != 2)
	cerr << "Usage: Simulation <placement policy>\n";
    else
    {
	MainBody m(argv[1]);
	m.Await(); // Suspend main thread. NOTE: This MUST be done by all applications.
    }

    return 0;
}


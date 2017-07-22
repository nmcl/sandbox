#include <iostream.h>
#include <Process.h>

#include "MainBody.h"

main(int argc, char* argv[])
{
    LWP_Thread::Initialize();

    if (argc != 2)
	cerr << "Usage: Simulation <placement policy>\n";
    else
    {
	MainBody m(argv[1]);
	m.Await();
    }

    return 0;
}


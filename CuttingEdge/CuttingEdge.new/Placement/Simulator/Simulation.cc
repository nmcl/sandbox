#include <iostream.h>
#include <Process.h>
#include "Machine.h"

#include "MainBody.h"

main(int argc, char* argv[])
{
    LWP_Thread::Initialize(); // Initialize threads package.

    if ((argc < 2) || ((argc > 2) && (argc != 7)))
    {
	cerr << "Usage: Simulation <placement policy>\n";
	cerr << "       Simulation <placement policy> <objects> <mean> <std deviation>\n";
	cerr << "                  <availability> <node number>\n";
	cerr << "\n<placement policy> : Random | Simple\n";
	cerr << "<objects>          : maximum number of objects in a group\n";
	cerr << "<mean>             : mean for MTTF and MTTR for nodes\n";
	cerr << "<std deviation>    : standard deviation MTTF and MTTR for nodes\n";
	cerr << "<availability>     : availability required from object group\n";
	cerr << "<node number>      : number of good nodes (out of " << MAX_MACHINES << ")\n";
    }
    else
    {
	char *tocreate = 0, *mean = 0, *sd = 0, *av = 0, *node = 0;
	if (argc > 2)
	{
	    tocreate = argv[2];
	    mean = argv[3];
	    sd = argv[4];
	    av = argv[5];
	    node = argv[6];
	}

	MainBody m(argv[1], tocreate, mean, sd, av, node);
	m.Await(); // Suspend main thread. NOTE: This MUST be done by all applications.
    }

    return 0;
}


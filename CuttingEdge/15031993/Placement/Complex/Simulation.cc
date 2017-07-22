#include <iostream.h>
#include <Process.h>
#include "Machine.h"
#include "MainBody.h"

main(int argc, char* argv[])
{
    GNU_Thread::Initialize(); // Initialize threads package.

    if ((argc < 3) || ((argc > 3) && (argc > 10)))
    {
	cerr << "Usage: Simulation <placement policy> <protocol>\n";
	cerr << "       Simulation <placement policy> <protocol> <objects>\n";
	cerr << "                  <mean> <std deviation>\n";
	cerr << "                  <availability> <MTTF/MTTR stats>\n";
	cerr << "\n<placement policy> : Random | Simple\n";
	cerr << "<protocol>         : Replication Protocol (AC | QC)\n";
	cerr << "<objects>          : maximum number of objects in a group\n";
	cerr << "<mean>             : mean for MTTF and MTTR for nodes\n";
	cerr << "<std deviation>    : standard deviation MTTF and MTTR for nodes\n";
	cerr << "<availability>     : availability desired from object group\n";
	cerr << "<rw_ratio>         : read/write ratio" << endl;
	cerr << "<rate>             : rate of operations" << endl;
	cerr << "<rnd hop>          : seed number generator" << endl;
    }
    else
    {
	MainBody m(argc, argv);
	m.Await(); // Suspend main thread. NOTE: This MUST be done by all applications.
    }

    return 0;
}


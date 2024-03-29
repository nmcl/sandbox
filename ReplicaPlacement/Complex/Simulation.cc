/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#include <Configure.h>

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif

#ifndef MAINBODY_H_
#  include "MainBody.h"
#endif


main(int argc, char* argv[])
{
    Thread_Type::Initialize(); // Initialize threads package.

    if ((argc < 3) || ((argc > 3) && (argc > 11)))
    {
	cerr << "Usage: Simulation <placement policy> <protocol>\n";
	cerr << "       Simulation <placement policy> <protocol> <objects>\n";
	cerr << "                  <mean> <std deviation>\n";
	cerr << "                  <availability> <rw_ratio>\n";
	cerr << "                  <clients> <rnd hop> <locktype>" << endl;
	cerr << "\n<placement policy> : Random | Simple\n";
	cerr << "<protocol>         : Replication Protocol (AC | QC | Passive | Single)\n";
	cerr << "<objects>          : maximum number of objects in a group\n";
	cerr << "<mean>             : mean for MTTF and MTTR for nodes\n";
	cerr << "<std deviation>    : standard deviation MTTF and MTTR for nodes\n";
	cerr << "<availability>     : availability desired from object group\n";
	cerr << "<rw_ratio>         : read/write ratio" << endl;
	cerr << "<clients>          : number of clients" << endl;
	cerr << "<rnd hop>          : seed number generator" << endl;
	cerr << "<locktype>         : type of lock to acquire" << endl;
    }
    else
    {
	MainBody m(argc, argv);
	m.Await(); // Suspend main thread. NOTE: This MUST be done by all applications.
    }

    return 0;
}


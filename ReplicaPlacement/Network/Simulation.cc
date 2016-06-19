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

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef MAINBODY_H_
#include "MainBody.h"
#endif


main(int argc, char* argv[])
{
    Thread_Type::Initialize(); // Initialize threads package.

    if ((argc < 2) || ((argc > 2) && (argc > 7)))
    {
	cerr << "Usage: Simulation <protocol>\n";
	cerr << "       Simulation <protocol> <objects>\n";
	cerr << "                  <mean> <std deviation>\n";
	cerr << "                  <clients> <locktype>" << endl;
	cerr << "<protocol>         : Replication Protocol (AC | QC)\n";
	cerr << "<objects>          : number of objects in a group\n";
	cerr << "<mean>             : mean for MTTF and MTTR for nodes\n";
	cerr << "<std deviation>    : standard deviation MTTF and MTTR for nodes\n";
	cerr << "<clients>          : number of clients" << endl;
	cerr << "<locktype>         : type of lock to acquire" << endl;
    }
    else
    {
	MainBody m(argc, argv);
	m.Await(); // Suspend main thread. NOTE: This MUST be done by all applications.
    }

    return 0;
}


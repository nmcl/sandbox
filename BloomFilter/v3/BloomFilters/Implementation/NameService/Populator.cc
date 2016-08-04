/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Populator.cc,v 1.1 1998/05/27 11:22:00 nmcl Exp $
 */

#include <System/string.h>
#include <System/iostream.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/stdio.h>
#include <System/signal.h>

/*
 * Do not re-order the include files!!
 */

#ifndef COMMON_ORB_H_
#  include <Common/Orb.h>
#endif

#ifndef COMMON_ORBSERVICES_H_
#  include <Common/ORBServices.h>
#endif

#ifndef COMMON_ORBINTERFACE_H_
#  include <Common/ORBInterface.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include <dummy_H.h>
#include <dummy_i.h>

static const char RCSid[] = "$Id: Populator.cc,v 1.1 1998/05/27 11:22:00 nmcl Exp $";

int main (int argc, char** argv)
{
    ORBInterface::initORB(argc, argv, DEFAULT_ORBID);
    ORBInterface::initBOA(argc, argv, DEFAULT_BOAID);

    int iter = 100000;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << ": [-size <number>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-size") == 0)
	{
	    iter = ::atoi(argv[i+1]);
	}
    }

    for (int j = 0; j < iter; j++)
    {
	Uid u;
	char* uidString = u.stringForm();
	dummy_i* impl = new dummy_i;
	READY_IMPL_(impl);
	dummy_ptr ptr = CORBA_REF_(dummy, impl);

	ORBServices::registerService(ptr, uidString, "dummy");

	cout << u << endl;

	::delete [] uidString;

	CORBA_(release)(ptr);

	DESTROY_IMPL_(impl);
    }

    
    cout << "Name service populated with " << iter << " objects." << endl;
    
    return 0;
}


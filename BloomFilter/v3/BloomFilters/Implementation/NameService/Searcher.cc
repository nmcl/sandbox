/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Searcher.cc,v 1.1 1998/05/27 11:22:00 nmcl Exp $
 */

#include <System/string.h>
#include <System/iostream.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/stdio.h>
#include <System/signal.h>
#include <System/sys/time.h>
#include <System/fstream.h>

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

static const char RCSid[] = "$Id: Searcher.cc,v 1.1 1998/05/27 11:22:00 nmcl Exp $";

int main (int argc, char** argv)
{
    ORBInterface::initORB(argc, argv, DEFAULT_ORBID);
    ORBInterface::initBOA(argc, argv, DEFAULT_BOAID);

    int fileSize = 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << ": -size <number> [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-size") == 0)
	{
	    fileSize = ::atoi(argv[i+1]);
	}
    }

    if (fileSize == 0)
    {
	cerr << "Error - no size of object ID file specified." << endl;
	return -1;
    }

    srand48(0);

    long long currentTime = 0;

    for (int j = 0; j < fileSize; j++)
    {
	ifstream input("names");
	int number = fileSize * drand48();
	char uid[128];
	struct timeval stime, ftime;

	::memset(uid, '\0', 128);

	for (int k = 0; k < number; k++)
	    input >> uid;

	::gettimeofday(&stime, NULL);

	CORBA_(Object_ptr) obj = ORBServices::getService(uid, "dummy");

	if (CORBA_(is_nil)(obj))
	{
	    cerr << "Error - object is NULL" << endl;
	    return -1;
	}

	::gettimeofday(&ftime, NULL);

	currentTime += ((ftime.tv_sec*1000000)+ftime.tv_usec) -
	    ((stime.tv_sec*1000000)+stime.tv_usec);
	
	CORBA_(release)(obj);
    }

    cout << "time taken in microseconds: " << currentTime << endl;
    
    return 0;
}


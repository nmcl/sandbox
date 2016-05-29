/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Searcher.cc,v 1.2 1998/07/17 09:00:34 nmcl Exp $
 */

#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <fstream.h>

/*
 * Do not re-order the include files!!
 */

#include <omniORB2/CORBA.h>

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include <dummy.h>

static const char RCSid[] = "$Id: Searcher.cc,v 1.2 1998/07/17 09:00:34 nmcl Exp $";

int main (int argc, char** argv)
{
    CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv, (ORBid) "omniORB2");
    CORBA::BOA_ptr boa = orb->BOA_init(argc, argv, (OAid) "omniORB2_BOA");    

    int fileSize = 0;
    char* fileName = "names";

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -number <number of entries> [-file <name>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-file") == 0)
	    fileName = argv[i+1];
	if (::strcmp(argv[i], "-number") == 0)
	    fileSize = ::atoi(argv[i+1]);
    }

    if (fileSize == 0)
    {
	cerr << "Error - no size of object ID file specified." << endl;
	return -1;
    }

    srand48(0);

    long long currentTime = 0;
    CORBA::Object_ptr initServ;
    CosNaming::NamingContext_ptr rootContext;
	
    try
    {
	initServ = orb->resolve_initial_references("NameService");
	
	try
	{
	    rootContext = CosNaming::NamingContext::_narrow(initServ);

	    if (CORBA::is_nil(rootContext))
		throw new BAD_PARAM();
	}
	catch (...)
	{
	    error_stream << WARNING << "Naming service narrow failed" << endl;

	    return -1;
	}

	CORBA::release(initServ);
    }
    catch (...)
    {
	error_stream << WARNING << "Caught unexpected exception!" << endl;
	
	return -2;
    }

    for (int j = 0; j < fileSize; j++)
    {
	ifstream input(fileName);
	int number = fileSize * drand48();
	char* uid = ::new char[128];
	struct timeval stime, ftime;

	::memset(uid, '\0', 128);

	if (number == 0)
	    number = 1;

	for (int k = 0; k < number; k++)
	    input >> uid;

	::gettimeofday(&stime, NULL);

	CORBA::Object_ptr obj = CORBA::Object::_nil();
	
	try
	{
	    CosNaming::Name contextName;

	    contextName.length(1);
	    
	    contextName[0].id = (const char*) uid;
	    contextName[0].kind = (const char*) "dummy";

	    obj = rootContext->resolve(contextName);
	}
	catch (CosNaming::NamingContext::NotFound& ex)
	{
	    cerr << "Not found exception" << endl;

	    obj = CORBA::Object::_nil();
	}
	catch (...)
	{
	    cerr << "Caught resolve exception" << endl;
	    
	    obj = CORBA::Object::_nil();
	}

	if (CORBA::is_nil(obj))
	{
	    cerr << "Error - object is NULL" << endl;
	    return -1;
	}

	::gettimeofday(&ftime, NULL);

	currentTime += ((ftime.tv_sec*1000000)+ftime.tv_usec) -
	    ((stime.tv_sec*1000000)+stime.tv_usec);

	::delete [] uid;
	
	CORBA::release(obj);
    }

    cout << "Average lookup time in milliseconds: "
	 << (double) (currentTime/fileSize)/1000 << endl;

    CORBA::release(rootContext);
    
    return 0;
}


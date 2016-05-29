/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Populator.cc,v 1.2 1998/07/17 09:00:32 nmcl Exp $
 */

#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

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
#include <dummy_i.h>

static const char RCSid[] = "$Id: Populator.cc,v 1.2 1998/07/17 09:00:32 nmcl Exp $";

int main (int argc, char** argv)
{
    CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv, (ORBid) "omniORB2");
    CORBA::BOA_ptr boa = orb->BOA_init(argc, argv, (OAid) "omniORB2_BOA");

    int iter = 100000;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << ": [-number <number entries>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-number") == 0)
	{
	    iter = ::atoi(argv[i+1]);
	}
    }

    for (int j = 0; j < iter; j++)
    {
	Uid u;
	char* uidString = u.stringForm();
	dummy_i* impl = new dummy_i;

	boa->obj_is_ready(impl);
	
	dummy_ptr objRef = impl->_this();
	CORBA::Object_var initServ = orb->resolve_initial_references("NameService");
	NamingContext_var rootContext;
	
	try
        {
	    rootContext = CosNaming::NamingContext::_narrow(initServ);

	    if (CORBA::is_nil(rootContext))
		throw new BAD_PARAM();
	}
	catch (...)
        {
	    error_stream << WARNING << "Naming service narrow failed" << endl;

	    return FALSE;
	}

	CosNaming::Name contextName;

	contextName.length(1);
	    
	contextName[0].id = (const char*) uidString;
	contextName[0].kind = (const char*) "dummy";
	
	rootContext->rebind(contextName, objRef);
	
	cout << u << endl;

	::delete [] uidString;

	CORBA::release(objRef);
	
	impl->_dispose();
    }
    
    //    cout << "Name service populated with " << iter << " objects." << endl;
    
    return 0;
}


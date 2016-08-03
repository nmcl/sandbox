/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TemplateTest.cc,v 1.6 1994/10/14 11:03:50 nmcl Exp $
 */


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMPLEXOBJECT_H_
#  include <Replication/COControl.h>
#endif

#ifndef RECARRAY_H_
#  include "RecArray.h"
#endif


ComplexObjectControl controller;


/*
 * This test will create a persistent array of persistent integer objects. The array
 * and all of its elements are to be replicated in exactly the same way, so we will
 * not create a separate group for each object within the naming service, but will
 * simply make use of template groups. The template will indicate where the replicas will
 * be placed and we simply need to associate the groups to be governed by the template
 * with it at the naming service. Caching is implicit with this as well.
 */

int main (int argc, char** argv)
{
    Boolean isNew = TRUE, finished = FALSE;
    AtomicAction* Z = 0;
    RecArray* X = 0;
    Uid* groupUid = 0;
    int res = -1;

    for (int i = 0; i < argc; i++)
    {
        if (::strcmp(argv[i], "-uid") == 0)
	{
	    isNew = FALSE;
	    groupUid = new Uid(argv[i+1]);
	}
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: ComplexTest [-help] [-uid <UID>]" << endl;
	    return 0;
	}
    }

    if (isNew)
    {
	X = new RecArray(res);

	if (res != 0)
	{
	    delete X;
	    return -1;
	}

	groupUid = new Uid(X->get_uid());

	cout << "group UID is " << *groupUid << endl;
    }
    else
    {
        int res = -1;

        X = new RecArray(*groupUid, res);

	if (res != 0)
	{
	    cerr << "An error occurred while re-creating object." << endl;
	    delete X;
	    return -1;
	}
	else
	    cout << "Recreated object successfully." << endl;
    }

    while (!finished)
    {
	char opt;

        cout << "\nMenu\n" << endl;
	cout << "A/a: Set array element." << endl;
	cout << "B/b: Get array element." << endl;
	cout << "C/c: Configure as single object." << endl;
	cout << "D/d: Configure as separate objects." << endl;
	cout << "Z/z: Quit." << endl;
	cin >> opt;

	if (Z)
	    delete Z;

	Z = new AtomicAction;

	switch(opt)
	{
	    case 'A':
	    case 'a':
	    {
	        int index, value;

		cout << "What is the element and value?" << endl;
		cin >> index >> value;

	        Z->Begin();

		if (X->setElement(index, value))
		{
		    if (Z->End() != COMMITTED)
		        cerr << "Commit error." << endl;
		    else
		        cout << "Value changed successfully." << endl;
		}
		else
		{
		    Z->Abort();
		    cerr << "An error occurred during set operation." << endl;
		}
	    }
	    break;
	    case 'B':
	    case 'b':
	    {
	        int index, value;

		cout << "What is the element to check?" << endl;
		cin >> index;

		Z->Begin();

		value = X->getElement(index);

		if (value >= 0)
		{
		    if (Z->End() != COMMITTED)
		        cerr << "Commit error." << endl;
		    else
		        cout << "Value returned was " << value << endl;
		}
		else
		{
		    Z->Abort();
		    cerr << "An error occurred in get operation." << endl;
		}
	    }
	    break;
	    case 'C':
	    case 'c':
	    case 'D':
	    case 'd':
	    {
	        Boolean result;

		if ((opt == 'C') || (opt == 'c'))
		    result = X->associate();
		else
		    result = X->disassociate();

		if (result)
		    cout << "Configured successfully." << endl;
		else
		    cerr << "Configuration error." << endl;
	    }
	    break;
	    case 'Z':
	    case 'z':
	        finished = TRUE;
	        if (Z)
		    delete Z;
	        break;
	    default:
	        break;
	}
    }

    delete X;
    return 0;
}

		
		


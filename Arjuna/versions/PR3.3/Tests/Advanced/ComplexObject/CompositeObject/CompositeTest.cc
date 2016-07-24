/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CompositeTest.cc,v 1.8 1995/07/03 14:30:10 ngdp Exp $
 */


#ifndef SYSV
#  ifndef SYS_TIME_H_
#    include <System/sys/time.h>
#  endif
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

#ifndef RECARRAY_H_
#  include "RecArray.h"
#endif


/* Number of elements in array. */

extern const int SIZE;


RecArray* X = 0;
Uid* groupUid = 0;

/*
 * This test will create a replicated persistent array of persistent integers.
 * Each member of the array, as well as the array itself, will be replicated and
 * form a separate group at the naming service. However, because these objects are
 * so closely associated, we shall "group" them together at the naming service for
 * caching purposes, i.e., whenever any member of this logical group is accessed,
 * all of the information for all of the members will be obtained and cached locally
 * to the user. This cuts down on subsequent access times.
 */

int main(int argc, char** argv)
{
    Boolean isNew = TRUE, finished = FALSE;
    AtomicAction* Z = 0;
    int i = 0;
    
#ifndef SYSV
    struct timeval stime, ftime;
#endif
    
    for (i = 0; i < argc; i++)
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
	int res = -1;
	
	X = new RecArray(res);

	if (res != 0)
	{
	    delete X;
	    return -1;
	}
	else
	    cout << "\nComposite object UID is " << X->get_uid() << endl;

	groupUid = new Uid(X->get_uid());
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
	cout << "E/e: Time configuration." << endl;
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

	        Z->Begin();

		if ((opt == 'C') || (opt == 'c'))
		    result = X->associate();
		else
		    result = X->disassociate();

		if (result)
		{
		    if (Z->End() != COMMITTED)
		        cerr << "Commit error." << endl;
		    else
		        cout << "Configured successfully." << endl;
		}
		else
		{
		    Z->Abort();
		    cerr << "Configuration error." << endl;
		}
	    }
	    break;
	    case 'E':
	    case 'e':
	    {
		Boolean result = TRUE;
		
		Z->Begin();
		
#ifndef SYSV
		::gettimeofday(&stime, 0);
#endif		
		for (i = 0; (i < SIZE) && (result); i++)
		    result = X->setElement(i, 1);

#ifndef SYSV
		::gettimeofday(&ftime, 0);
#endif		
		if (result)
		{
		    if (Z->End() != COMMITTED)
		        cerr << "Commit error." << endl;
		    else
		    {
		        cout << "Timed successfully." << endl;
#ifndef SYSV
			cout << "Time for " << SIZE << " objects is:" << endl;
			cout << (ftime.tv_sec*1000000 + ftime.tv_usec) -
			        (stime.tv_sec*1000000 + stime.tv_usec) << " usecs" << endl;
#endif
		    }
		}
		else
		{
		    Z->Abort();
		    cerr << "An error occurred during timing operation." << endl;
		}
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

		
		


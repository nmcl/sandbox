/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test.cc,v 1.4 1994/11/29 14:02:17 ngdp Exp $
 */

#include "Object.h"

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

extern "C"
{
    void sleep(int);
}


int main (int argc, char** argv)
{
    AtomicAction A;
    TopLevelAction TL;
    Boolean lock;
    int res = 0;
    Uid* u = 0;
    Object *x = 0, *y = 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-uid") == 0)
	    u = new Uid(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: Test [-uid <uid>] [-help]" << endl;
	    return 0;
	}
    }

    x = new Object(res);
    
    if (u)
	y = new Object(*u, res);
    else
    {
	y = new Object(res);
	cout << "Uid of y is " << y->get_uid() << endl;
    }
    
    A.Begin();
    cout << "A.Begin : Started\n";

    cout << "\nSetting value of object 1 to 1\n";
    lock = x->change(1);
    if (!lock)
    {
	cout << "Could not acquire lock\n";
	A.Abort();
    }
    else
    {
	res = x->value();

	if (res == -1)
	    cerr << "An error has occurred while attempting to read object state\n" << flush;
	else
	{
	    if (res == 1)
		cout << "Value returned is 1. This is correct.\n" << flush;
	    else
		cerr << "An error has occurred. The state returned is " << res << " and not 1\n" << flush;
	}

	TL.Begin();
	cout << "\nTL.Begin() : Started (nested top-level atomic action)\n";

	cout << "\nSetting value of object 2 to 2\n";
	lock = y->change(2);
	if (!lock)
	{
	    cout << "Could not acquire lock\n";
	    TL.Abort();
	    A.Abort();
	}
	else
	{
	    res = y->value();

	    if (res == -1)
		cerr << "An error has occurred while attempting to read object state\n" << flush;
	    else
	    {
		if (res == 2)
		cout << "Value returned is 2. This is correct.\n" << flush;
		else
		    cerr << "An error has occurred. The state returned is " << res << " and not 2\n" << flush;
	    }

	    cout << "\nCommitting TL\n";

	    if (TL.End() != COMMITTED)
		cerr << "Error. Could not commit TL action" << endl;
	    else
		cout << "Committed TL action ok" << endl;

	    cout << "Will now abort A" << endl;

	    A.Abort();

	    res = x->value();

	    if (res == -1)
		cerr << "An error has occurred while attempting to read object 1 state\n" << flush;
	    else
	    {
		if (res == 0)
		    cout << "Final value returned from object 1 is 0. This is correct.\n" << flush;
		else
		    cerr << "An error has occurred. The state returned from object 1 is " << res << " and not 0\n" << flush;
	    }

	    res = y->value();
	    
	    if (res == -1)
		cerr << "An error has occurred while attempting to read object 2 state\n" << flush;
	    else
	    {
		if (res == 2)
		    cout << "Final value returned from object 2 is 2. This is correct.\n" << flush;
		else
		    cerr << "An error has occurred. The state returned from object 2 is " << res << " and not 2\n" << flush;
	    }
	}
    }

#ifdef DEBUG
    cout << "Sleeping for 100 seconds" << endl;
    sleep(100);
#endif
    
    delete x;
    delete y;

    return 0;
}

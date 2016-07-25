/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test.cc,v 1.1 1993/05/17 15:53:05 nmcl Exp $
 */

#include "Object.h"

#include <System/iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

main()
{
    AtomicAction A;
    TopLevelAction TL;
    Boolean lock;
    int res = 0;

    Object* x = new Object(res);
    Object* y = new Object(res);

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

    delete x;
    delete y;
}

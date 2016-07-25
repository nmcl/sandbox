/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: ObjectDriver.cc,v 1.1 1993/11/03 14:58:04 nmcl Exp $
 */

#include "Object_stub.h"

#include <Configure.h>

#include <iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int, char**)
#else
int main(int, char**)
#endif
{
    TopLevelAction TL1, TL2;
    AtomicAction A;
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

	cout << "Starting first nested top-level action" << endl;
	
	TL1.Begin();
	cout << "\nTL1.Begin() : Started\n";

	cout << "\nSetting value of object 1 to 2\n";
	lock = x->change(2);
	if (!lock)
	    cout << "Could not acquire lock. This is correct.\n";

	cout << "Aborting TL1" << endl;
	TL1.Abort();
	
	cout << "TL1.Abort() : Done" << endl;
	
	res = x->value();

	if (res == -1)
	    cerr << "An error has occurred while attempting to read object 1 state\n" << flush;
	else
	{
	    if (res == 1)
		cout << "Value returned is 1. This is correct.\n" << flush;
	    else
		cerr << "An error has occurred. The state returned is " << res << " and not 2\n" << flush;
	}

	cout << "Starting second nested top-level action" << endl;
	
	TL2.Begin();
	cout << "TL2.Begin() : Started" << endl;
	
	cout << "Setting value of object 2 to 2" << endl;
	
	lock = y->change(2);
	if (!lock)
	{
	    cout << "Could not acquire lock" << endl;
	    TL2.Abort();
	    A.Abort();
	}
	else
	{
	    cout << "Changed state ok" << endl;
	    cout << "Committing nested top-level action" << endl;
	    
	    if (TL2.End() != COMMITTED)
	    {
		cout << "TL2.End() : Not Done" << endl;
		A.Abort();
	    }
	    else
	    {
		cout << "TL2.End() : Done" << endl;
	    

		if (A.End() != COMMITTED)
		    cout << "\nA.End() : Not Done\n";
		else
		{
		    cout << "\nA.End() : Done\n";
		    res = x->value();

		    if (res == -1)
			cerr << "An error has occurred while attempting to read object state\n" << flush;
		    else
		    {
			if (res == 1)
			    cout << "Final value returned from object 1 is 1. This is correct.\n" << flush;
			else
			    cerr << "An error has occurred. The state returned from object 1 is " << res << " and not 1\n" << flush;
			
			res = y->value();
			
			if (res == -1)
			    cerr << "An error has occurred while attempting to read object state\n" << flush;
			else
			{
			    if (res == 2)
				cout << "Final value returned from object 2 is 2. This is correct.\n" << flush;
			    else
				cerr << "An error has occurred. The state returned from object 2 is " << res << " and not 2\n" << flush;
			}
		    }
		}
	    }
	}
    }

    delete x;
    delete y;

    return 0;
}

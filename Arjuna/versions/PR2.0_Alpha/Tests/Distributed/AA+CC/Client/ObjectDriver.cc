/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: ObjectDriver.cc,v 1.6 1993/03/18 15:05:26 n048z Exp $
 */

#include "Object_stub.h"

#include <Configure.h>

#include <iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    AtomicAction A, B;
    Boolean lock;
    int res = 0;

    Object* x = new Object(res);
    Uid u = x->get_uid();

    A.Begin();
    cout << "A.Begin : Started\n";

    cout << "\nSetting value to 1\n";
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

	B.Begin();
	cout << "\nB.Begin() : Started\n";

	cout << "\nSetting value to 2\n";
	lock = x->change(2);
	if (!lock)
	{
	    cout << "Could not acquire lock\n";
	    B.Abort();
	    A.Abort();
	}
	else
	{
	    res = x->value();

	    if (res == -1)
		cerr << "An error has occurred while attempting to read object state\n" << flush;
	    else
	    {
		if (res == 2)
		cout << "Value returned is 2. This is correct.\n" << flush;
		else
		    cerr << "An error has occurred. The state returned is " << res << " and not 2\n" << flush;
	    }

	    cout << "\nAborting B\n";
	    B.Abort();

	    if (A.End() != COMMITTED)
		cout << "\nA.End() : Not Done\n";
	    else
	    {
		res = x->value();
		cout << "\nA.End() : Done\n";

		if (res == -1)
		    cerr << "An error has occurred while attempting to read object state\n" << flush;
		else
		{
		    if (res == 1)
			cout << "Final value returned is 1. This is correct.\n" << flush;
		    else
			cerr << "An error has occurred. The state returned is " << res << " and not 1\n" << flush;
		}
	    }
	}
    }

    delete x;
    cout << "\nLoading object from ObjectStore\n";

    res = 0;
    x = new Object(u, res);

    if (res == 0)
	cout << "Constructor error\n";
    else
    {
	AtomicAction C;

	C.Begin();
	cout << "C.Begin : Started\n";

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

	if (C.End() != COMMITTED)
	    cout << "C.End() : Not Done\n";
	else
	    cout << "C.End() : Done\n";
    }
    
    delete x;

    return 0;
}

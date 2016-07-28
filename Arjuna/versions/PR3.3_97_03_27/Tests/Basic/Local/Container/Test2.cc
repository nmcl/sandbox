/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test2.cc,v 1.1 1994/01/20 12:01:04 ngdp Exp $
 */

#include "Object.h"

#include <System/iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

main()
{
    AtomicAction A,B;
    Boolean lock;
    int res = 0;

    Object* x = new Object(res);
    Uid u = x->get_uid();

    A.Begin();
    cout << "A.Begin : Started" << endl;

    cout << "\nSetting value to 1" << endl;
    lock = x->change(1);
    if (!lock)
    {
	cout << "Could not acquire lock" << endl;
	A.Abort();
    }
    else
    {
	B.Begin();
	cout << "B.Begin : Started" << endl;
	res = x->value();

	if (res == -1)
	    cerr << "An error has occurred while attempting to read object state" << endl;
	else
	{
	    if (res == 1)
		cout << "Value returned is 1. This is correct." << endl;
	    else
		cerr << "An error has occurred. The state returned is " << res << " and not 1" << endl;
	}

	

	delete x;

	if (B.Abort() != ABORTED)
	    cout << "\nB.Abort() : Not Done" << endl;
	else
	{
	    cout << "\nB.Abort() : Done" << endl;

	}

	if (A.End() != COMMITTED)
	    cout << "\nA.End() : Not Done" << endl;
	else
	{
	    cout << "\nA.End() : Done" << endl;

	}
    }
    
}

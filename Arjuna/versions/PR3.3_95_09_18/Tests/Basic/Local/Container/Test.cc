/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test.cc,v 1.3 1994/12/06 09:27:14 ngdp Exp $
 */

#include "Container.h"
#include "Object.h"

#include <System/iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

main()
{
    AtomicAction A, B;
    Boolean ok;
    
    int res = 0;

    Container *c = new Container;
    Object* x = new Object(res);
    Uid u = x->get_uid();
    

    A.Begin();
    
    cout << "A.Begin : Started" << endl;
    cout << "Setting value to 1" << endl;

    ok = x->change(1);
    if (!ok)
    {
	cout << "Could not acquire lock on object" << endl;
	A.Abort();
    }
    else
    {
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

	B.Begin();

	cout << "B.Begin() : Started" << endl;
	cout << "Setting value to 2" << endl;

	ok = x->change(2);
	if (!ok)
	{
	    cout << "Could not acquire lock on object" << endl;
	    B.Abort();
	    A.Abort();
	}
	else
	{
	    cout << "Inserting into container" << endl;
	    ok = c->insert(x);
	    if (!ok)
	    {
		cout << "Could not insert object into container" << endl;
		B.Abort();
		A.Abort();
	    }
	    else
	    {
		cout << "Aborting B" << endl;

		B.Abort();

		if (A.End() != COMMITTED)
		    cout << "A.End() : Not Done" << endl;
		else
		{
		    cout << "A.End() : Done" << endl;
		}
	    }
	}
    }
    
    delete c;  

    cout << "Loading object from ObjectStore" << endl;

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
	    cout << "C.End() : Done\n";    }
    
    delete x;

    return 0;
}

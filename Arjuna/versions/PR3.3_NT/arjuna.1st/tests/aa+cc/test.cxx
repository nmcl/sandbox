/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test.cc,v 1.2 1994/06/13 14:08:21 nmcl Exp $
 */

#include <System/fstream.h>
#include <System/iostream.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

#include "Object.h"

int main (int, char**)
{
	cout << "Starting to run" << endl;

	ofstream output("C:/temp/id");

    AtomicAction A, B;
    Boolean lock;
    int res = 0;

	cout << "creating object" << endl;

    Object* x = new Object(res);
    Uid u = x->get_uid();

	output << u << endl;

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
		if (!x->value(res))
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
			if (!x->value(res))
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

			x->value(res);

			cout << "Current value is now " << res << "\n" << flush;

			if (A.End() != COMMITTED)
				cout << "\nA.End() : Not Done\n";
			else
			{
				cout << "\nA.End() : Done\n";

				if (!x->value(res))
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

		if (!x->value(res))
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

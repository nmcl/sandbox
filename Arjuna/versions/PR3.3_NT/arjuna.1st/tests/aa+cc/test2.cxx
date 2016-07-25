/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Test2.cc,v 1.3 1994/06/13 14:08:22 nmcl Exp $
 */

#include "Object.h"

#include <System/iostream.h>
#include <System/fstream.h>
#include <System/string.h>

#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

int main (int, char**)
{
    AtomicAction A;
    int res = 0, number = 0;
	Boolean lock = FALSE;
	char uidBuff[128];

	::memset(uidBuff, '\0', 128);

	ifstream input("C:/temp/id");

	input >> uidBuff;

	Uid u(uidBuff);

    Object* x = new Object(u, res);

    A.Begin();
    cout << "A.Begin : Started" << endl;

	lock = x->value(number);

	if (lock)
		cout << "value is " << number << endl;

	/*
    cout << "\nSetting value to 10" << endl;
    lock = x->change(10);
	*/

    if (!lock)
    {
		cout << "Could not acquire lock" << endl;
		A.Abort();
    }
    else
    {
		cout << "Got lock. Will now sleep" << endl;
//		sleep(10);

		if (A.End() != COMMITTED)
			cout << "\nA.End() : Not Done" << endl;
		else
		{
			cout << "\nA.End() : Done" << endl;

		}
    }

	delete x;

    return 0;
}

/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CounterSetup.cc,v 1.3 1996/09/07 14:44:45 arjuna Exp $
 */

#include <System/stdlib.h>
#include <System/iostream.h>
#include <System/string.h>

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#include <Config/Configure.h>

#include <arjuna.h>

#include "LocalNewStudentRecordNumber.h"

void usage (char* name)
{
    cout << "Usage: " << name << " [-help] [-value] [-reset <number>]" << endl;
}


int main (int argc, char** argv)
{
    SOutCome res = UNKNOWN;
    NewStudentRecordNumber* foo = 0;
    Boolean reset = FALSE;
    int number = 0;
    Boolean inspect = FALSE;
    Uid u(counterUid);

    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-value") == 0)
		inspect = TRUE;
	    else
	    {
		if (::strcmp(argv[i], "-reset") == 0)
		{
		    number = atoi(argv[i+1]);
		    i++;
		    reset = TRUE;
		}
		else
		{
		    cout << "Unknown option " << argv[i] << endl;
		    usage(argv[0]);
			
		    return -1;
		}
	    }
	}
    }

    if (!inspect && !reset)
    {
	foo = new NewStudentRecordNumber(res, 0);

	if (res == DONE)
	{
	    ObjectStore* toUse = ObjectStore::create(DEFAULT_OBJECTSTORE_Q);
	    ObjectState* state = toUse->read_committed(foo->get_uid(), foo->type());
	
	    if (state)
	    {
		if (!toUse->write_committed(u, foo->type(), *state))
		    cout << "\nError while attempting to write state." << endl;
		else
		{
		    toUse->remove_committed(foo->get_uid(), foo->type());
		    cout << "Counter created ok." << endl;
		}
	    }
	    else
		cout << "\nError - could not read state." << endl;
	}
	else
	    cout << "Counter creation error." << endl;
    }
    else
    {
	foo = new NewStudentRecordNumber(res, u);

	if (res == DONE)
	{
	    res = UNKNOWN;
	    
	    if (reset)
	    {
		foo->setNumber(res, number);
		if (res == DONE)
		    cout << "Reset ok." << endl;
		else
		    cout << "Reset error." << endl;
	    }
	    else
	    {
		foo->getNumber(res, number);
		if (res == DONE)
		    cout << "Value is " << number << endl;
		else
		    cout << "Get error." << endl;
	    }
	}
	else
	    cout << "Counter creation error." << endl;
    }

    delete foo;
}

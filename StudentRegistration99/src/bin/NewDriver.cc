/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NewDriver.cc,v 1.3 1996/08/29 12:34:12 arjuna Exp $
 */

#include <Common/Boolean.h>
#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>

#include <iostream.h>
#include <string.h>

#include <local.h>
#include <Requests.h>
#include <Defs.h>

extern AtomicAction* action;
extern void CleanUp ();

char RecordLocked[STUDENTNUMBERSIZE +1];

int main (int argc, char** argv)
{
    char* number = (char*) 0;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-help] -number <number>" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-number") == 0)
	    number = argv[i+1];
    }

    if (!number)
    {
	cerr << "Error - no number given." << endl;
	return -1;
    }

    ::memset(RecordLocked, '\0', STUDENTNUMBERSIZE+1);
    ::memcpy(RecordLocked, NORECORD, STUDENTNUMBERSIZE);
    
    char Reply[1024];

    if (DBNew(number, Reply) == 0)
    {
	cout << "Reply is " << Reply << endl;
	action->End();
    }
    else
	cout << "New error." << endl;

    CleanUp();
    
    return 0;
}


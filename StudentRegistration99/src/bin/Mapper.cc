/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Mapper.cc,v 1.3 1996/08/29 12:34:12 arjuna Exp $
 */

#include <Common/Boolean.h>
#include <Common/Uid.h>

#include <iostream.h>
#include <string.h>

#include <local.h>

extern Boolean mapId (const char* number, Uid& id);

char* RecordLocked = NULL;
char Username[9];

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

    ::strcpy(Username, DEBUG_USER);
    
    Uid id(NIL_UID);

    if (mapId(number, id))
	cout << "Uid is " << id << endl;
    else
	cout << "A map error occurred" << endl;

    return 0;
}


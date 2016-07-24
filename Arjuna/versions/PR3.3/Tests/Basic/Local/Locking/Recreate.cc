/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Recreate.cc,v 1.2 1995/07/03 14:30:19 ngdp Exp $
 */

#include "Object.h"

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_B_
#  include <System/stdlib.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif


#define UID_SIZE 128

int main (int argc, char** argv)
{
    AtomicAction A;
    int res = 0;
    Boolean finished = FALSE;
    char* fileName = "output";
    char UidString[UID_SIZE], dummy[UID_SIZE];
    int i = 0;
    
    for (i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-file") == 0)
	    fileName = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: Test [-file <file>] [-help]" << endl;
	    return 0;
	}
    }

    ifstream input(fileName);

    if (!input)
    {
	cout << "Error. Could not open file " << fileName << endl;
	return -1;
    }

    A.Begin();

    // recreate the objects

    i = 0;

    while (!finished)
    {
	::memset(UidString, '\0', UID_SIZE);

	if (input.peek() != EOF)
	{
	    input >> dummy >> UidString;

	    cout << "got " << UidString << endl;
	    cout << "size " << strlen(UidString) << endl;

	    if (::strlen(UidString) > 0)
	    {
		Uid u(UidString);
	
		Object* x = new Object(u, res);

		// set lock

		cout << "Locking object " << i+1 << endl;
		i++;
	
		if (x->change(1) == FALSE)
		    cout << "Lock error!" << endl;
	    }
	    else
		finished = TRUE;
	}
	else
	    finished = TRUE;
    }
    
    return 0;
}

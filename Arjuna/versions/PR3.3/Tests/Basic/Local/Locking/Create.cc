/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Create.cc,v 1.2 1995/07/03 14:30:18 ngdp Exp $
 */

#include "Object.h"

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
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


int main (int argc, char** argv)
{
    AtomicAction A, B;
    int res = 0, numberOfObjects = 100;
    char* fileName = "output";
    Object** objectList = 0;
    int i = 0;
	
    for (i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-file") == 0)
	    fileName = argv[i+1];
	if (::strcmp(argv[i], "-number") == 0)
	    numberOfObjects = atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: Create [-file <file>] [-number <number>] [-help]" << endl;
	    return 0;
	}
    }

    objectList = new Object* [numberOfObjects];
    
    ofstream output(fileName);
    
    if (!output)
    {
	cout << "Error. Could not open file " << fileName << endl;
	return -1;
    }

    A.Begin();

    // create the objects
    
    for (i = 0; i < numberOfObjects; i++)
    {
	objectList[i] = new Object(res);

	/*
	 * We want all but one of the locks to be released,	so
	 * only puy N-1 of the ids into the file.
	 */

	if (i != 0)
	    output << objectList[i]->type() << " " << objectList[i]->get_uid() << endl;

	// set lock

	cout << "Locking object " << i+1 << endl;
	
	if (objectList[i]->change(1) == FALSE)
	    cout << "Lock error!" << endl;
    }

    // end this action to make sure states are in object store

    A.End();

    B.Begin();

    cout << "About to recreate objects" << endl;

    for (i = 0; i < numberOfObjects; i++)
    {
	cout << "Locking object " << i+1 << endl;
	
	if (objectList[i]->change(1) == FALSE)
	    cout << "Lock error!" << endl;
    }
    
    // now loop

    for (;;);

    // put this here to avoid warnings!
    
    return 0;
}

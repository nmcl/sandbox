/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unlock_shmem.cc,v 1.4 1995/06/27 14:32:24 ngdp Exp $
 */

/*
 * Tool for unlocking shared memory given type and uid
 */


#include <Config/Configure.h>

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
#endif

#ifndef LOCKSTORE_H_
#  include "../../../ClassLib/include/LockStore.h"
#endif

int main (int argc, char** argv)
{
    Boolean gotUid = FALSE;
	
    char uidString[80];
    ::memset(uidString, '\0', 80);
    char* objectType = 0;
    
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -Type type -Uid uid" << endl;
	    exit(0);
	}
	if (::strcmp(argv[i], "-Uid") == 0)
	{
	    ::strcpy(uidString, argv[i+1]);
	    gotUid = TRUE;
	}
	if (::strcmp(argv[i], "-Type") == 0)
	    objectType = argv[i+1];
    }

    if (!gotUid || (objectType == 0) )
    {
	// We've don't have enough info
	cout << "Usage: " << argv[0] << " -Type type -Uid uid" << endl;
	exit(0);
    }
    else
    {
	Boolean result;
	
        Uid objectUid(uidString);
	cout << "Attempting to unlock object:" << endl;
	cout << "\tObject uid = " << objectUid << endl;
	cout << "\tObject type = " << objectType << endl;

	// Get the shmem key
	key_t shmemKey = typeToKey((const TypeName) objectType);
	cout << "Shared memory key is " << shmemKey << endl;

	// Create the Semaphore and LockStore
	Semaphore *sem = new Semaphore(shmemKey);
	LockStore *ls = new LockStore(shmemKey);

	sem->wait();

	result = ls->remove_state(objectUid, objectType);
	delete ls;

	sem->signal();

	delete sem;

	if (result)
	{
	    cout << "Lock successfully removed" << endl;
	}
	else
	{
	    cout << "*WARNING* Lock removal failed" << endl;
	}
	
    }

    return (0);
    
}

	

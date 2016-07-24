/*
 * Copyright (C) 1996
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionMonitor.cc,v 1.1 1996/09/25 14:55:07 nmcl Exp $
 */

#include <System/unistd.h>
#include <System/stdlib.h>
#include <System/stdio.h>
#include <System/iostream.h>
#include <System/errno.h>
#include <System/string.h>

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#include <ObjectStore/LocalOSTypes.h>
#include <Replication/RemoteOSTypes.h>

void usage (const char* name)
{
    cout << "Usage: " << name << " [-store <object store>] [-root <store root>] [-help]" << endl;
}

int main (int argc, char** argv)
{
    const char* storeImple = (char*) 0;
    const char* root = (char*) 0;

    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-store") == 0)
	    {
		storeImple = argv[i+1];
		i++;
	    }
	    else
	    {
		if (::strcmp(argv[i], "-root") == 0)
		{
		    root = argv[i+1];
		    i++;
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

    if (!storeImple)
	storeImple = DEFAULT_OBJECTSTORE_Q;

    ObjectStore* imple = ObjectStore::create((char*) storeImple, root);

    if (!imple)
    {
	cout << "Error - could not create object store " << storeImple << endl;
	return -1;
    }

    ObjectState types;
    
    if (imple->allTypes(types))
    {
	char* theName = (char*) 0;
	int count = 0;

	while ((types.unpack(theName) && (::strcmp(theName, "") != 0)))
	{
	    count++;
	    
	    cout << count << ": " << theName << endl;

	    ObjectState uids;
	    
	    if (imple->allObjUids(theName, uids))
	    {
		Uid theUid;

		while (theUid.unpack(uids) && (theUid != NIL_UID))
		    cout << "\t" << theUid << " state is "
			 << (ObjectStore::StateStatus) imple->currentState(theUid, theName) << endl;

		cout << endl;
	    }
	    
	    ::delete [] theName;
	}
    }

    ObjectStore::destroy(imple);
    
    return 0;
}


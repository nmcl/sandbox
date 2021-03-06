/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OSConverter.cc,
 */

#include <Configure.h>

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef LSPOBJSTORE_H_
#  include <Arjuna/LSPObjStore.h>
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    ObjectStore *oldStore = 0, *newStore = 0;
    Uid* stateUid = 0;
    Boolean found = FALSE, inverse = FALSE;
    ObjectState* osState = 0;
    char tn[MAXPATHLEN];

    for (int i = 0; (i < argc); i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: OSConverter -TypeName type -oldUid uid [-inverse]\n" << flush;
	    exit(0);
	}

	if (::strcmp(argv[i], "-TypeName") == 0)
	{
	    ::memset(tn, '\0', MAXPATHLEN);
	    ::memcpy(tn, argv[i+1], ::strlen(argv[i+1]));
	    found = TRUE;
	}

	if (::strcmp(argv[i], "-oldUid") == 0)
	    stateUid = new Uid(argv[i+1]);

	if (::strcmp(argv[i], "-inverse") == 0)
	    inverse = TRUE;
    }

    if ((!stateUid) || (!found))
    {
	cout << "Usage: OSConverter -TypeName type -oldUid uid [-inverse]\n" << flush;
	exit(-1);
    }

    if (!inverse)
    {
	oldStore = ObjectStore::create("LocalSimplePersistentObjectStore");
	newStore = ObjectStore::create("LocalPersistentObjectStore");
    }
    else
    {
	oldStore = ObjectStore::create("LocalPersistentObjectStore");
	newStore = ObjectStore::create("LocalSimplePersistentObjectStore");
    }

    osState = oldStore->read_state(*stateUid, tn);
    if (!osState)
    {
	cout << "Error. Could not read state\n" << flush;
	return -1;
    }

    if (!oldStore->write_state(*stateUid, tn, *osState))
    {
	cout << "Error. Could not create backup of old state\n" << flush;
	return -2;
    }

    if (inverse)
    {
	if (newStore->write_state(*stateUid, tn, *osState))
	{
	    (void) oldStore->remove_uncommitted(*stateUid, tn);
	    (void) newStore->commit_state(*stateUid, tn);
	    cout << "Converted state ok\n" << flush;
	    return 2;
	}
    }
    else
    {
	if (newStore->write_committed(*stateUid, tn, *osState))
	{
	    (void) oldStore->remove_uncommitted(*stateUid, tn);
	    (void) oldStore->commit_state(*stateUid, tn);
	    cout << "Converted state ok\n" << flush;
	    return 2;
	    
	}
    }

    cout << "Error. Could not write new state\n" << flush;
    return -3;
}

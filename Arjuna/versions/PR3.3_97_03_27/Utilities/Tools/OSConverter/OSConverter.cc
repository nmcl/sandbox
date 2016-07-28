/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OSConverter.cc,v 1.7 1995/02/22 14:56:19 ngdp Exp $
 */

#include <Config/Configure.h>

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

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif


const TypeName defaultToStore = FragmentedStoreType;
const TypeName defaultFromStore = ShadowStoreType;

int main(int argc, char** argv)
{
    ObjectStore *oldStore = 0, *newStore = 0;
    Uid* stateUid = 0;
    Boolean found = FALSE, inverse = FALSE;
    ObjectState* osState = 0;
    char tn[MAXPATHLEN];
    char* storeRoot = (char*) 0;
    TypeName storeFrom = defaultFromStore;
    TypeName storeTo = defaultToStore;

    for (int i = 0; (i < argc); i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: OSConverter -TypeName type -oldUid uid [-inverse] [-storepath <store path>] ";
	    cout << "[-from <store type>] [-to <store type>]" << endl;
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
	if (::strcmp(argv[i], "-storepath") == 0)
	    storeRoot = argv[i+1];
	if (::strcmp(argv[i], "-from") == 0)
	    storeFrom = argv[i+1];
	if (::strcmp(argv[i], "-to") == 0)
	    storeTo = argv[i+1];
    }

    if ((!stateUid) || (!found))
    {
	cout << "Usage: OSConverter -TypeName type -oldUid uid [-inverse] [-store <store path>] ";
	cout << "[-from <store type>] [-to <store type>]" << endl;
	exit(-1);
    }

    if (!inverse)
    {
	oldStore = ObjectStore::create(storeFrom, storeRoot);
	newStore = ObjectStore::create(storeTo, storeRoot);
    }
    else
    {
	oldStore = ObjectStore::create(storeTo, storeRoot);
	newStore = ObjectStore::create(storeFrom, storeRoot);
    }

    osState = oldStore->read_committed(*stateUid, tn);
    if (!osState)
    {
	cout << "Error. Could not read state\n" << flush;
	return -1;
    }

    if (newStore->write_committed(*stateUid, tn, *osState))
    {
	(void) oldStore->remove_uncommitted(*stateUid, tn);
	cout << "Converted state ok\n" << flush;
	return 2;
    }

    cout << "Error. Could not write new state\n" << flush;
    return -3;
}

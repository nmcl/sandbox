/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateChecker.cc,v 1.1 1996/09/03 15:20:53 arjuna Exp $
 */

#include <System/strstream.h>
#include <System/iostream.h>
#include <System/string.h>
#include <System/stdlib.h>

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICACONFIGURATION_H_
#  include <Replication/ReplicaConfiguration.h>
#endif

#include <Replication/RemoteOSTypes.h>
#include <ObjectStore/ObjectStoreNames.h>

#include <local.h>
#include <arjuna.h>

void usage (const char* name)
{
    cout << "Usage: " << name << " -uid <uid> [-help]" << endl;
}

    
int main (int argc, char** argv)
{
    char* uidString = (char*) 0;
    Boolean result = FALSE;
    ReplicaConfiguration conf(ObjectStoreConfiguration, result);
    int i;

    if (!result)
    {
	cout << "Error with replica configuration file " << ObjectStoreConfiguration << endl;
	return -1;
    }
        
    for (i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-uid") == 0)
	    {
		uidString = argv[i+1];
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

    if (!uidString)
    {
	cout << "You must supply a Uid" << endl;
	usage(argv[0]);
	
	return -1;
    }

    Uid u(uidString);
    ObjectStore** objStore = new ObjectStore* [conf.number()];
    ObjectState** states = new ObjectState* [conf.number()];
    
    for (i = 0; i < conf.number(); i++)
    {
	char* name[1];

	name[0] = conf.hosts()[i];

	objStore[i] = ObjectStore::create(ReplicatedStoreType);

	if (!objStore[i])
	{
	    cout << "Error - could not create object store " <<ReplicatedStoreType << endl;
	    return -1;
	}

	objStore[i]->storeLocation(name, 1);
	objStore[i]->useStoreLocation(TRUE);
    }

    /*
     * Should have all replicas by now, but some may be unavailable.
     */

    const TypeName typeName = "/StateManager/LockManager/StudentRecord";
    int committing = 0;
    ObjectState* state1 = 0;
    
    for (i = 0; i < conf.number(); i++)
    {
	/*
	 * Try to read uncommitted first.
	 */
	
	states[i] = objStore[i]->read_uncommitted(u, typeName);

	if (states[i])
	{
	    state1 = states[i];
	    committing++;
	}
    }

    if ((committing == conf.number()) || (committing == 0))
    {
	if (committing == conf.number())
	    cout << "All " << committing << " states are shadowed." << endl;
	else
	    cout << "All " << conf.number() << " states are committed." << endl;
    }
    else
    {
	cout << "There are " << committing << " shadowed states and "
	     << conf.number() << " committed." << endl;
	
	/*
	 * Otherwise check them all to see if some have been
	 * committed.
	 */

	ObjectState** committed = new ObjectState* [conf.number()];
	ObjectState* state2 = 0;
	ObjectState* state3 = 0;
	
	for (i = 0; i < conf.number(); i++)
	{
	    committed[i] = objStore[i]->read_committed(u, typeName);

	    if (!states[i])
		state2 = committed[i];
	    else
		state3 = committed[i];
	}

	if (!state1 || !state2 || !state3)
	{
	    cerr << "Fatal state error!" << endl;
	    abort();
	}

	Boolean shouldCommit = FALSE;
	
	if (state2->length() == state1->length())
	    shouldCommit = (Boolean) (memcmp(state1->buffer(), state2->buffer(), state1->length()) == 0);

	if (shouldCommit)
	    cout << "All shadowed states are the same as the committed states." << endl;
	else
	{
	    cout << "Shadowed states are different." << endl;
	    
	    if (state2->length() == state3->length())
	    {
		if (memcmp(state2->buffer(), state3->buffer(), state2->length()) == 0)
		    cout << "The old states are the same as the current committed states." << endl;
		else
		    cout << "The old states are different to the current committed states." << endl;
	    }
	}

	for (i = 0; i < conf.number(); i++)
	{
	    if (committed[i])
		delete committed[i];
	}

	delete [] committed;
    }

    for (i = 0; i < conf.number(); i++)
    {
	if (objStore[i])
	    delete objStore[i];
	if (objStore[i])
	    delete states[i];
    }

    delete [] objStore;
    delete [] states;
    
    return 0;
}

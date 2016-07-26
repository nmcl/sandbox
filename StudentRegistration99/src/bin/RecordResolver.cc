/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordResolver.cc,v 1.1 1996/09/03 15:20:52 arjuna Exp $
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

#include "LocalStudentRecord.h"

#include <local.h>
#include <arjuna.h>

void usage (const char* name)
{
    cerr << "Usage: " << name << " -uid <uid> [-help]" << endl;
}

    
int main (int argc, char** argv)
{
    char* uidString = (char*) 0;
    Boolean result = FALSE;
    ReplicaConfiguration conf(ObjectStoreConfiguration, result);
    int i;

    if (!result)
    {
	cerr << "Error with replica configuration file " << ObjectStoreConfiguration << endl;
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
		cerr << "Unknown option " << argv[i] << endl;
		usage(argv[0]);
		
		return -1;
	    }
	}
    }

    if (!uidString)
    {
	cerr << "You must supply a Uid" << endl;
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
	    cerr << "Error - could not create object store " <<ReplicatedStoreType << endl;
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

    /*
     * If all states are uncommitted then make them all committed.
     */

    if ((committing == conf.number()) || (committing == 0))
    {
	if (committing == conf.number())
	{
	    cerr << "Will commit states." << endl;
	    
	    for (i = 0; i < conf.number(); i++)
		(void) objStore[i]->commit_state(u, typeName);
	}
	else
	    cerr << "All states committed. Nothing to be done." << endl;
    }
    else
    {
	/*
	 * Otherwise check them all to see if some have been
	 * committed.
	 */

	ObjectState** committed = new ObjectState* [conf.number()];
	ObjectState* state2 = 0;
	
	for (i = 0; i < conf.number(); i++)
	{
	    committed[i] = objStore[i]->read_committed(u, typeName);

	    if (!states[i])
		state2 = committed[i];
	}

	if (!state1 || !state2)
	{
	    cerr << "Fatal state error!" << endl;
	    abort();
	}
	
	Boolean shouldCommit = FALSE;

	if (state2->length() == state1->length())
	    shouldCommit = (Boolean) (memcmp(state1->buffer(), state2->buffer(), state1->length()) == 0);

	for (i = 0; i < conf.number(); i++)
	{
	    if (states[i])
	    {
		if (shouldCommit)
		{
		    cerr << "Will commit states." << endl;
		    (void) objStore[i]->commit_state(u, typeName);
		}
		else
		{
		    cerr << "Will abort states." << endl;
		    (void) objStore[i]->remove_uncommitted(u, typeName);
		}
	    }

	    if (committed[i])
		delete committed[i];
	}

	delete [] committed;
    }

    for (i = 0; i < conf.number(); i++)
    {
	if (objStore[i])
	    delete objStore[i];
	if (states[i])
	    delete states[i];
    }

    delete [] objStore;
    delete [] states;

    SOutCome res = UNKNOWN;
    
    StudentRecord sr(res, u);

    if (res == DONE)
    {
	Buffer record;
	
	sr.fetchRecord (res, record);
	
	if (res == DONE)
	{
	    void*  wholeLine = NULL;
	    size_t length = 0;

	    if (record.unpack(wholeLine, length))
	    {
		char* ptr = ::new char[length+1];
		::memset(ptr, '\0', length+1);
		::memcpy(ptr, wholeLine, length);
		cout << ptr << endl;
		::delete [] ptr;
	    }
	    else
		cerr << "**STUDENT RECORD EXTRACT ERROR**" << endl;

	    if (wholeLine)
		delete wholeLine;
	}
    }
    else	
	cerr << "**STUDENT RECORD EXTRACT ERROR**" << endl;
    
    return 0;
}

    
	    
	


	

	
    
	
	
